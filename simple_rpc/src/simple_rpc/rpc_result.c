#include "simple_rpc/rpc_result.h"

struct RPCResult rpc_result_ok(struct RPCValue value)
{
    return (struct RPCResult){.is_error = false, value = value};
}

struct RPCResult rpc_result_error(struct RPCValue value)
{
    return (struct RPCResult){.is_error = true, value = value};
}
