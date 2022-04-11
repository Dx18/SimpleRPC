#pragma once

#include <sys/socket.h>

#include "util/byte_buffer.h"

#include "simple_rpc/rpc_procedure.h"

/**
 * Runs Simple-RPC server on given address. Information about
 * procedures is stored in `procedures` array. Length of this array is
 * `procedure_count`. Returns -1 on failure (error is stored in
 * `errno`). Otherwise returns 0.
 */
int run_server(
    const struct sockaddr* addr,
    size_t addr_len,
    const struct RPCProcedure* procedures,
    size_t procedure_count);
