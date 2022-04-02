#pragma once

#include "util/byte_buffer.h"

#include "simple_rpc/rpc_value.h"

void rpc_none_serialize(struct MutableByteBuffer* buffer);

void rpc_int_serialize(int value, struct MutableByteBuffer* buffer);

void rpc_string_serialize(const char* value, struct MutableByteBuffer* buffer);

void rpc_value_serialize(
    struct RPCValue value,
    struct MutableByteBuffer* buffer);
