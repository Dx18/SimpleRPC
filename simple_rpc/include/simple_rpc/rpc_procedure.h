#pragma once

#include "util/byte_buffer.h"

/** Information about RPC procedure/function stored on server-side. */
struct RPCProcedure {
    /** Name of procedure. */
    const char* name;
    /**
     * Procedure caller. Accepts buffer from which it would
     * deserialize arguments and index where deserialization would
     * start (see `rpc_value_deserialize`) and returns result of
     * operation.
     */
    struct RPCValue (*caller)(const struct MutableByteBuffer*, size_t*);
};
