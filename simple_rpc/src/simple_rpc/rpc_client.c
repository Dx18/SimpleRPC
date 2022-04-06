#include "simple_rpc/rpc_client.h"

#include <arpa/inet.h>
#include <unistd.h>

#include "simple_rpc/rpc_deserialize.h"
#include "simple_rpc/rpc_result.h"
#include "simple_rpc/rpc_serialize.h"
#include "simple_rpc/rpc_value.h"
#include "util/byte_buffer.h"

void rpc_client_init(
    struct RPCClient* client,
    const struct sockaddr* address,
    size_t address_length)
{
    client->address = malloc(address_length);
    *client->address = *address;
    client->address_len = address_length;
}

void rpc_client_destroy(struct RPCClient* client)
{
    free(client->address);
}

int rpc_client_init_ipv4(
    struct RPCClient* client,
    const char* address,
    uint16_t port)
{
    struct in_addr address_converted;
    if (!inet_pton(AF_INET, address, &address_converted)) {
        return 0;
    }
    struct sockaddr_in address_full = {
        .sin_family = AF_INET,
        .sin_port = htons(port),
        .sin_addr = address_converted};
    rpc_client_init(
        client, (const struct sockaddr*)&address_full, sizeof(address_full));
    return 1;
}

int rpc_client_init_ipv6(
    struct RPCClient* client,
    const char* address,
    uint16_t port)
{
    struct in6_addr address_converted;
    if (!inet_pton(AF_INET, address, &address_converted)) {
        return 0;
    }
    struct sockaddr_in6 address_full = {
        .sin6_family = AF_INET6,
        .sin6_port = htons(port),
        .sin6_addr = address_converted,
    };
    rpc_client_init(
        client, (const struct sockaddr*)&address_full, sizeof(address_full));
    return 1;
}

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
    const struct RPCClient* rpc_client,
    const char* procedure_name,
    struct RPCValue* args,
    size_t arg_count)
{
    int client = socket(AF_INET, SOCK_STREAM, 0);
    if (client == -1) {
        return rpc_result_error(rpc_string("Could not creaate client socket"));
    }

    if (connect(client, rpc_client->address, rpc_client->address_len) == -1) {
        close(client);
        return rpc_result_error(rpc_string("Could not connect to server"));
    }

    struct RPCValue procedure_name_value = rpc_string(procedure_name);

    if (write_rpc_values(client, &procedure_name_value, 1) == -1 ||
        write_rpc_values(client, args, arg_count) == -1) {
        shutdown(client, SHUT_RDWR);
        close(client);
        return rpc_result_error(rpc_string("Could not write to socket"));
    }

    shutdown(client, SHUT_WR);

    struct MutableByteBuffer buffer;
    mut_byte_buffer_init(&buffer);
    if (mut_byte_buffer_read_until_eof(&buffer, client) == -1) {
        mut_byte_buffer_destroy(&buffer);
        shutdown(client, SHUT_RD);
        close(client);
        return rpc_result_error(rpc_string("Could not read response"));
    }

    size_t index = 0;

    struct RPCValue is_error_flag;
    if (rpc_value_deserialize(&buffer, &index, &is_error_flag) !=
        kRPCDeserializeResultOk) {
        return rpc_result_error(rpc_string("Could not deserialize error flag"));
    } else if (is_error_flag.type != kRPCValueTypeInt) {
        return rpc_result_error(rpc_string("Server sent non-int error flag"));
    }

    bool is_error = is_error_flag.value_int;
    rpc_value_destroy(&is_error_flag);

    struct RPCValue value;
    if (rpc_value_deserialize(&buffer, &index, &value)) {
        return rpc_result_error(
            rpc_string("Could not deserialize result value"));
    }

    if (index != mut_byte_buffer_length(&buffer)) {
        mut_byte_buffer_destroy(&buffer);
        rpc_value_destroy(&value);
        shutdown(client, SHUT_RD);
        close(client);
        return rpc_result_error(rpc_string("Server sent additional data"));
    }

    mut_byte_buffer_destroy(&buffer);

    return is_error ? rpc_result_error(value) : rpc_result_ok(value);
}

struct RPCResult
rpc_call(const struct RPCClient* rpc_client, const char* procedure_name, ...)
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
        return rpc_call_array(rpc_client, procedure_name, NULL, 0);
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

    return rpc_call_array(rpc_client, procedure_name, values, arg_count);
}
