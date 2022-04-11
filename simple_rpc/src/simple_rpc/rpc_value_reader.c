#include "simple_rpc/rpc_value_reader.h"

void rpc_value_reader_init(
    struct RPCValueReader* reader,
    const struct MutableByteBuffer* buffer)
{
    reader->buffer = buffer;
    reader->index = 0;
}

bool rpc_value_reader_is_end(const struct RPCValueReader* reader)
{
    return reader->index == mut_byte_buffer_length(reader->buffer);
}

enum RPCDeserializeResult rpc_value_reader_read_next(
    struct RPCValueReader* reader,
    struct RPCValue* out_result)
{
    return rpc_value_deserialize(reader->buffer, &reader->index, out_result);
}
