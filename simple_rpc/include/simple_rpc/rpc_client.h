#pragma once

#include <stdarg.h>

#include <sys/socket.h>

#include "simple_rpc/rpc_result.h"
#include "simple_rpc/rpc_value.h"

#include "util/byte_buffer.h"
#include "util/byte_buffer_io.h"

/**
 * Simple-RPC client which stores information about Simple-RPC server.
 */
struct RPCClient {
    /** Address of server. */
    struct sockaddr* address;
    /** Length of address. */
    size_t address_len;
};

/**
 * Initializes client with given address. Allocates memory for address
 * using `malloc`.
 */
void rpc_client_init(
    struct RPCClient* client,
    const struct sockaddr* address,
    size_t address_length);

/** Destroys client by freeing memory allocated to server address. */
void rpc_client_destroy(struct RPCClient* client);

/**
 * Initializes client with given IPv4 address. Allocates memory for
 * address using `malloc`.
 */
int rpc_client_init_ipv4(
    struct RPCClient* client,
    const char* address,
    uint16_t port);

/**
 * Initializes client with given IPv6 address. Allocates memory for
 * address using `malloc`.
 */
int rpc_client_init_ipv6(
    struct RPCClient* client,
    const char* address,
    uint16_t port);

/**
 * Performs call. Name of procedure is passed in `procedure_name`,
 * arguments are passed in `args` array (length of this array is
 * `arg_count`).
 */
struct RPCResult rpc_call_array(
    const struct RPCClient* rpc_client,
    const char* procedure_name,
    struct RPCValue* args,
    size_t arg_count);

/**
 * Performs call. Name of procedure is passed in `procedure_name`,
 * arguments are passed as variable arguments (last argument must be
 * empty RPC value, it is not counted as an argument).
 */
struct RPCResult
rpc_call(const struct RPCClient* rpc_client, const char* procedure_name, ...);
