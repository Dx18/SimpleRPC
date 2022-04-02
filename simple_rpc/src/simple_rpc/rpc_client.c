#include "simple_rpc/rpc_client.h"

#include <unistd.h>

#include "simple_rpc/rpc_deserialize.h"
#include "simple_rpc/rpc_serialize.h"
#include "simple_rpc/rpc_value.h"
#include "util/byte_buffer.h"

static int write_rpc_values(int fd, struct RPCValue* values, size_t count)
{
    struct MutableByteBuffer buffer;
    mut_byte_buffer_init(&buffer);

    for (size_t i = 0; i < count; ++i) {
        rpc_value_serialize(values[i], &buffer);
    }

    int result = mut_byte_buffer_write(&buffer, fd);

    mut_byte_buffer_destroy(&buffer);

    return result;
}

struct RPCResult rpc_call_array(
    const struct sockaddr* addr,
    size_t addr_len,
    const char* procedure_name,
    struct RPCValue* args,
    size_t arg_count)
{
    int client = socket(AF_INET, SOCK_STREAM, 0);
    if (client == -1) {
        return (struct RPCResult){
            .is_error = 1,
            .value = rpc_string("Could not create client socket")};
    }

    if (connect(client, addr, addr_len) == -1) {
        close(client);
        return (struct RPCResult){
            .is_error = 1, .value = rpc_string("Could not connect to server")};
    }

    struct RPCValue procedure_name_value = rpc_string(procedure_name);

    if (write_rpc_values(client, &procedure_name_value, 1) == -1 ||
        write_rpc_values(client, args, arg_count) == -1) {
        shutdown(client, SHUT_RDWR);
        close(client);
        return (struct RPCResult){
            .is_error = 1, .value = rpc_string("Could not write to socket")};
    }

    shutdown(client, SHUT_WR);

    struct MutableByteBuffer buffer;
    mut_byte_buffer_init(&buffer);
    if (mut_byte_buffer_read_until_eof(&buffer, client) == -1) {
        mut_byte_buffer_destroy(&buffer);
        shutdown(client, SHUT_RD);
        close(client);
        return (struct RPCResult){
            .is_error = 1, .value = rpc_string("Could not read response")};
    }

    struct RPCValue response;
    size_t index = 0;
    enum RPCDeserializeResult deserialize_result =
        rpc_value_deserialize(&buffer, &index, &response);

    if (index != mut_byte_buffer_length(&buffer)) {
        mut_byte_buffer_destroy(&buffer);
        rpc_value_destroy(&response);
        shutdown(client, SHUT_RD);
        close(client);
        return (struct RPCResult){
            .is_error = 1, .value = rpc_string("Server sent additional data")};
    }

    mut_byte_buffer_destroy(&buffer);

    return (struct RPCResult){.is_error = 0, .value = response};
}

struct RPCResult rpc_call(
    const struct sockaddr* addr,
    size_t addr_len,
    const char* procedure_name,
    ...)
{
    va_list args;

    size_t arg_count = 0;

    va_start(args, procedure_name);
    while (1) {
        struct RPCValue value = va_arg(args, struct RPCValue);
        if (value.type == kRPCValueTypeNone) {
            break;
        }
        ++arg_count;
    }
    va_end(args);

    if (arg_count == 0) {
        return rpc_call_array(addr, addr_len, procedure_name, NULL, 0);
    }

    struct RPCValue values[arg_count];

    va_start(args, procedure_name);
    for (size_t i = 0; i < arg_count; ++i) {
        struct RPCValue value = va_arg(args, struct RPCValue);
        if (value.type == kRPCValueTypeNone) {
            break;
        }
        values[i] = value;
    }
    va_end(args);

    return rpc_call_array(addr, addr_len, procedure_name, values, arg_count);
}
