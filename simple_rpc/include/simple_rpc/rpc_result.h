#pragma once

#include <stdbool.h>

#include "rpc_value.h"

struct RPCResult {
    bool is_error;
    struct RPCValue value;
};

struct RPCResult rpc_result_ok(struct RPCValue value);

struct RPCResult rpc_result_error(struct RPCValue value);
