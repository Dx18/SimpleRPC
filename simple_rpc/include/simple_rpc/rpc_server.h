#pragma once

#include <sys/socket.h>

#include "util/byte_buffer.h"

#include "simple_rpc/rpc_procedure.h"

int run_server(
    const struct sockaddr* addr,
    size_t addr_len,
    const struct RPCProcedure* procedures,
    size_t procedure_count);
