#pragma once

#include <stdarg.h>

#include <sys/socket.h>

#include "simple_rpc/rpc_value.h"

#include "util/byte_buffer.h"
#include "util/byte_buffer_io.h"

struct RPCClient {
    struct sockaddr* address;
    size_t address_len;
};

struct RPCResult {
    int is_error;
    struct RPCValue value;
};

void rpc_client_init(
    struct RPCClient* client,
    const struct sockaddr* address,
    size_t address_length);

void rpc_client_destroy(struct RPCClient* client);

int rpc_client_init_ipv4(
    struct RPCClient* client,
    const char* address,
    uint16_t port);

int rpc_client_init_ipv6(
    struct RPCClient* client,
    const char* address,
    uint16_t port);

struct RPCResult rpc_call_array(
    const struct RPCClient* rpc_client,
    const char* procedure_name,
    struct RPCValue* args,
    size_t arg_count);

struct RPCResult
rpc_call(const struct RPCClient* rpc_client, const char* procedure_name, ...);
