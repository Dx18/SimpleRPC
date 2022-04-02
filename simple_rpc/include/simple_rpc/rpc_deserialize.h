#pragma once

#include <stdlib.h>

#include "util/byte_buffer.h"

#include "simple_rpc/rpc_value.h"

enum RPCDeserializeResult {
    kRPCDeserializeResultOk = 0,
    kRPCDeserializeResultError = 1,
};

enum RPCDeserializeResult
rpc_none_deserialize(const struct MutableByteBuffer* buffer, size_t* index);

enum RPCDeserializeResult rpc_int_deserialize(
    const struct MutableByteBuffer* buffer,
    size_t* index,
    int* out_result);

enum RPCDeserializeResult rpc_string_deserialize(
    const struct MutableByteBuffer* buffer,
    size_t* index,
    char** out_result);

enum RPCDeserializeResult rpc_value_deserialize(
    const struct MutableByteBuffer* buffer,
    size_t* index,
    struct RPCValue* out_result);
