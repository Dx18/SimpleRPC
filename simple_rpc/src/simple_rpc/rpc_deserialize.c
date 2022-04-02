#include "simple_rpc/rpc_deserialize.h"

#include <string.h>

enum RPCDeserializeResult
rpc_none_deserialize(const struct MutableByteBuffer* buffer, size_t* index)
{
    return kRPCDeserializeResultOk;
}

enum RPCDeserializeResult rpc_int_deserialize(
    const struct MutableByteBuffer* buffer,
    size_t* index,
    int* out_result)
{
    if (mut_byte_buffer_length(buffer) - *index < sizeof(int)) {
        return kRPCDeserializeResultError;
    }

    *out_result = *((int*)(mut_byte_buffer_ptr(buffer) + *index));
    *index += sizeof(int);
    return kRPCDeserializeResultOk;
}

enum RPCDeserializeResult rpc_string_deserialize(
    const struct MutableByteBuffer* buffer,
    size_t* index,
    char** out_result)
{
    const uint8_t* ptr = mut_byte_buffer_ptr(buffer) + *index;
    size_t available_length = mut_byte_buffer_length(buffer) - *index;

    const uint8_t* zero_byte_ptr =
        (const uint8_t*)memchr(ptr, '\0', available_length);

    if (zero_byte_ptr == NULL) {
        return kRPCDeserializeResultError;
    }

    *out_result = (char*)malloc(zero_byte_ptr - ptr + 1);
    strcpy(*out_result, (char*)ptr);
    *index += strlen(*out_result) + 1;

    return kRPCDeserializeResultOk;
}

enum RPCDeserializeResult rpc_value_deserialize(
    const struct MutableByteBuffer* buffer,
    size_t* index,
    struct RPCValue* out_result)
{
    if (*index >= buffer->length) {
        return kRPCDeserializeResultError;
    }

    uint8_t type = mut_byte_buffer_ptr(buffer)[*index];
    ++*index;

    enum RPCDeserializeResult result = kRPCDeserializeResultError;

    switch (type) {
    case kRPCValueTypeNone: {
        result = rpc_none_deserialize(buffer, index);
        if (result != kRPCDeserializeResultError) {
            *out_result = rpc_none();
        }
        break;
    }
    case kRPCValueTypeInt: {
        int value_int;
        result = rpc_int_deserialize(buffer, index, &value_int);
        if (result != kRPCDeserializeResultError) {
            *out_result = rpc_int(value_int);
        }
        break;
    }
    case kRPCValueTypeString: {
        char* value_string;
        result = rpc_string_deserialize(buffer, index, &value_string);
        if (result != kRPCDeserializeResultError) {
            *out_result = rpc_allocated_string(value_string);
        }
        break;
    }
    }

    return result;
}
