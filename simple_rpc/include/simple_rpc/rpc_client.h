#pragma once

#include <stdarg.h>

#include <sys/socket.h>

#include "simple_rpc/rpc_value.h"

#include "util/byte_buffer.h"
#include "util/byte_buffer_io.h"

struct RPCResult {
    int is_error;
    struct RPCValue value;
};

struct RPCResult rpc_call_array(
    const struct sockaddr* addr,
    size_t addr_len,
    const char* procedure_name,
    struct RPCValue* args,
    size_t arg_count);

struct RPCResult rpc_call(
    const struct sockaddr* addr,
    size_t addr_len,
    const char* procedure_name,
    ...);
