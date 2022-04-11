#pragma once

#include "util/byte_buffer.h"

#include "simple_rpc/rpc_value.h"

/**
 * Serializes empty RPC value to given buffer. Does not write type
 * tag.
 */
void rpc_none_serialize(struct MutableByteBuffer* buffer);

/**
 * Serializes integer RPC value to given buffer. Does not write type
 * tag.
 */
void rpc_int_serialize(int value, struct MutableByteBuffer* buffer);

/**
 * Serializes string RPC value to given buffer. Does not write type
 * tag.
 */
void rpc_string_serialize(const char* value, struct MutableByteBuffer* buffer);

/** Serializes RPC value to given buffer. Writes type tag. */
void rpc_value_serialize(
    struct RPCValue value,
    struct MutableByteBuffer* buffer);
