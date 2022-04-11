#pragma once

#include <stdbool.h>

#include "rpc_value.h"

/** Result of RPC operation. */
struct RPCResult {
    /** Indicates whether RPC operation failed. */
    bool is_error;
    /**
     * Result of RPC operation (exact meaning depends on `is_error`
     * flag).
     */
    struct RPCValue value;
};

/** Creates successful result of RPC operation. */
struct RPCResult rpc_result_ok(struct RPCValue value);

/** Creates unsuccessful result of RPC operation. */
struct RPCResult rpc_result_error(struct RPCValue value);
