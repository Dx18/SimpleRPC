#include "simple_rpc/rpc_server.h"

#include <stdlib.h>
#include <string.h>

#include <unistd.h>

#include "util/byte_buffer.h"
#include "util/byte_buffer_io.h"

#include "simple_rpc/rpc_deserialize.h"
#include "simple_rpc/rpc_procedure.h"
#include "simple_rpc/rpc_serialize.h"
#include "simple_rpc/rpc_value.h"

static struct RPCValue handle_request(
    const struct MutableByteBuffer* request,
    const struct RPCProcedure* procedures,
    size_t procedure_count)
{
    size_t index = 0;

    struct RPCValue procedure_name;
    if (rpc_value_deserialize(request, &index, &procedure_name) !=
        kRPCDeserializeResultOk) {
        return rpc_string("Could not deserialize procedure name");
    }

    if (procedure_name.type != kRPCValueTypeString) {
        rpc_value_destroy(&procedure_name);
        return rpc_string("Name of procedure must be string");
    }

    for (size_t i = 0; i < procedure_count; ++i) {
        if (strcmp(procedure_name.value_string, procedures[i].name) == 0) {
            rpc_value_destroy(&procedure_name);

            struct RPCValue value = procedures[i].caller(request, &index);
            if (index != mut_byte_buffer_length(request)) {
                rpc_value_destroy(&value);
                return rpc_string("Client sent additional information");
            }

            return value;
        }
    }

    rpc_value_destroy(&procedure_name);
    return rpc_string("Unknown procedure");
}

static void handle_client(
    int client,
    const struct RPCProcedure* procedures,
    size_t procedure_count)
{
    struct MutableByteBuffer buffer;
    mut_byte_buffer_init(&buffer);

    if (mut_byte_buffer_read_until_eof(&buffer, client) == -1) {
        mut_byte_buffer_destroy(&buffer);
        return;
    }

    struct RPCValue response =
        handle_request(&buffer, procedures, procedure_count);

    mut_byte_buffer_destroy(&buffer);

    struct MutableByteBuffer response_buffer;
    mut_byte_buffer_init(&response_buffer);

    rpc_value_serialize(response, &response_buffer);

    rpc_value_destroy(&response);

    mut_byte_buffer_write(&response_buffer, client);
    mut_byte_buffer_destroy(&response_buffer);
}

int run_server(
    const struct sockaddr* addr,
    size_t addr_len,
    const struct RPCProcedure* procedures,
    size_t procedure_count)
{
    int server = socket(AF_INET, SOCK_STREAM, 0);
    if (server == -1) {
        return -1;
    }

    if (bind(server, addr, addr_len) == -1) {
        close(server);
        return EXIT_FAILURE;
    }

    listen(server, SOMAXCONN);

    while (1) {
        int client = accept(server, NULL, NULL);
        if (client == -1) {
            continue;
        }

        handle_client(client, procedures, procedure_count);
        shutdown(client, SHUT_RDWR);
        close(client);
    }

    shutdown(server, SHUT_RDWR);
    close(server);

    return EXIT_SUCCESS;
}
