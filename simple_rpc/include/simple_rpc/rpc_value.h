#pragma once

#include <stdbool.h>

/** Type of RPC value. */
enum RPCValueType {
    /** No value. */
    kRPCValueTypeNone = 0,
    /** Integer (platform-dependent). */
    kRPCValueTypeInt = 1,
    /** String. */
    kRPCValueTypeString = 2,
};

/** RPC value. */
struct RPCValue {
    /** Type of stored value. */
    enum RPCValueType type;
    union {
        /** Integer value (if `type` is `kRPCValueTypeInt`). */
        int value_int;
        struct {
            /** String value (if `type` is `kRPCValueTypeString`). */
            const char* value_string;
            /**
             * Indicated whether string was allocated using `malloc`.
             */
            bool value_string_allocated;
        };
    };
};

/** Constructs empty RPC value. */
struct RPCValue rpc_none();

/** Constructs integer RPC value. */
struct RPCValue rpc_int(int value);

/** Constructs string RPC value. */
struct RPCValue rpc_string(const char* value);

/**
 * Constructs allocated string RPC value. Does not make any
 * allocations. It is assumed that string is already allocated.
 */
struct RPCValue rpc_allocated_string(const char* value);

/** Destroyes RPC value by freeing memory if needed. */
void rpc_value_destroy(const struct RPCValue* value);
