#include "simple_rpc/rpc_value.h"

#include <stdlib.h>

struct RPCValue rpc_none()
{
    return (struct RPCValue){.type = kRPCValueTypeNone};
}

struct RPCValue rpc_int(int value)
{
    return (struct RPCValue){.type = kRPCValueTypeInt, .value_int = value};
}

struct RPCValue rpc_string(const char* value)
{
    return (struct RPCValue){
        .type = kRPCValueTypeString,
        .value_string = value,
        .value_string_allocated = false};
}

struct RPCValue rpc_allocated_string(const char* value)
{
    return (struct RPCValue){
        .type = kRPCValueTypeString,
        .value_string = value,
        .value_string_allocated = true};
}

void rpc_value_destroy(const struct RPCValue* value)
{
    switch (value->type) {
    case kRPCValueTypeNone:
    case kRPCValueTypeInt:
        break;
    case kRPCValueTypeString:
        if (value->value_string_allocated) {
            free((char*)value->value_string);
        }
        break;
    }
}
