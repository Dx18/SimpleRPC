#pragma once

#include "simple_rpc/rpc_deserialize.h"

/**
 * Reader used to deserialized RPC values sequentially stored in
 * mutable byte buffer.
 */
struct RPCValueReader {
    const struct MutableByteBuffer* buffer;
    size_t index;
};

/** Initializes RPC value reader with given buffer. */
void rpc_value_reader_init(
    struct RPCValueReader* reader,
    const struct MutableByteBuffer* buffer);

/** Returns true if there are no RPC values available to read. */
bool rpc_value_reader_is_end(const struct RPCValueReader* reader);

/** Deserializes next RPC value and returns deserialization result. */
enum RPCDeserializeResult rpc_value_reader_read_next(
    struct RPCValueReader* reader,
    struct RPCValue* result);
