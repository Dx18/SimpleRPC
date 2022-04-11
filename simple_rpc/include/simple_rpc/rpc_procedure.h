#pragma once

#include "util/byte_buffer.h"

#include "simple_rpc/rpc_value_reader.h"

/** Information about RPC procedure/function stored on server-side. */
struct RPCProcedure {
    /** Name of procedure. */
    const char* name;
    /**
     * Procedure caller. Accepts reader for arguments reading and
     * returns result of operation.
     */
    struct RPCValue (*caller)(struct RPCValueReader*);
};
