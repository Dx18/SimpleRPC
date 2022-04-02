#pragma once

enum RPCValueType {
    kRPCValueTypeNone = 0,
    kRPCValueTypeInt = 1,
    kRPCValueTypeString = 2,
};

struct RPCValue {
    enum RPCValueType type;
    union {
        int value_int;
        struct {
            const char* value_string;
            int value_string_allocated;
        };
    };
};

struct RPCValue rpc_none();

struct RPCValue rpc_int(int value);

struct RPCValue rpc_string(const char* value);

struct RPCValue rpc_allocated_string(const char* value);

void rpc_value_destroy(const struct RPCValue* value);
