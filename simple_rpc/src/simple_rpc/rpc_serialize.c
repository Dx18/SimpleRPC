#include "simple_rpc/rpc_serialize.h"

void rpc_none_serialize(struct MutableByteBuffer* buffer)
{
}

void rpc_int_serialize(int value, struct MutableByteBuffer* buffer)
{
    mut_byte_buffer_append_buffer(buffer, &value, sizeof(int));
}

void rpc_string_serialize(const char* value, struct MutableByteBuffer* buffer)
{
    mut_byte_buffer_append_str(buffer, value);
}

void rpc_value_serialize(
    struct RPCValue value,
    struct MutableByteBuffer* buffer)
{
    mut_byte_buffer_append_byte(buffer, value.type);
    switch (value.type) {
    case kRPCValueTypeNone:
        rpc_none_serialize(buffer);
        break;
    case kRPCValueTypeInt:
        rpc_int_serialize(value.value_int, buffer);
        break;
    case kRPCValueTypeString:
        rpc_string_serialize(value.value_string, buffer);
        break;
    }
}
