#pragma once

#include <stdlib.h>

#include "util/byte_buffer.h"

#include "simple_rpc/rpc_value.h"

/** Result of RPC value deserialization. */
enum RPCDeserializeResult {
    /** Deserialization succeeded. */
    kRPCDeserializeResultOk = 0,
    /** Deserialization failed. */
    kRPCDeserializeResultError = 1,
};

/**
 * Deserializes empty RPC value from given buffer from byte with index
 * `*index` (`*index` "points" to actual stored value without value
 * tag). Returns whether operation was successful. Adjusts `*index` on
 * success (see `rpc_value_deserialize`).
 */
enum RPCDeserializeResult
rpc_none_deserialize(const struct MutableByteBuffer* buffer, size_t* index);

/**
 * Deserializes integer RPC value from given buffer from byte with index
 * `*index` (`*index` "points" to actual stored value without value
 * tag). Returns whether operation was successful. Adjusts `*index` on
 * success (see `rpc_value_deserialize`).
 */
enum RPCDeserializeResult rpc_int_deserialize(
    const struct MutableByteBuffer* buffer,
    size_t* index,
    int* out_result);

/**
 * Deserializes string RPC value from given buffer from byte with
 * index `*index` (`*index` "points" to actual stored value without
 * value tag). Allocates read string. Returns whether operation was
 * successful. For `*index` adjustment rules see
 * `rpc_value_deserialize`.
 */
enum RPCDeserializeResult rpc_string_deserialize(
    const struct MutableByteBuffer* buffer,
    size_t* index,
    char** out_result);

/**
 * Deserializes RPC value from given buffer from byte with index
 * `*index`. Returns whether operation was successful. If operation
 * was successful adjusts index to it "points" to location right after
 * deserialized value.
 */
enum RPCDeserializeResult rpc_value_deserialize(
    const struct MutableByteBuffer* buffer,
    size_t* index,
    struct RPCValue* out_result);
