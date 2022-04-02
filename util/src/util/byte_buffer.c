#include "util/byte_buffer.h"

#include <assert.h>
#include <stdint.h>
#include <string.h>

#include <unistd.h>

const size_t kByteBufferMinPositiveCapacity = sizeof(void*);

void mut_byte_buffer_init(struct MutableByteBuffer* buffer)
{
    buffer->ptr = NULL;
    buffer->capacity = 0;
    buffer->length = 0;
}

void mut_byte_buffer_destroy(struct MutableByteBuffer* buffer)
{
    free(buffer->ptr);
}

void mut_byte_buffer_extend_reallocate(
    struct MutableByteBuffer* buffer,
    size_t required_capacity)
{
    if (buffer->capacity >= required_capacity) {
        return;
    }

    if (required_capacity < kByteBufferMinPositiveCapacity) {
        required_capacity = kByteBufferMinPositiveCapacity;
    }

    size_t new_capacity = buffer->capacity == 0 ? kByteBufferMinPositiveCapacity
                                                : buffer->capacity;
    while (new_capacity < required_capacity) {
        new_capacity *= 2;
    }
    uint8_t* new_ptr = malloc(new_capacity);
    if (buffer->ptr != NULL) {
        memcpy(new_ptr, buffer->ptr, buffer->length);
        free(buffer->ptr);
    }

    buffer->ptr = new_ptr;
    buffer->capacity = new_capacity;
}

void mut_byte_buffer_append_buffer(
    struct MutableByteBuffer* buffer,
    const void* buffer_to_append,
    size_t length)
{
    size_t required_capacity = buffer->length + length + 1;
    mut_byte_buffer_extend_reallocate(buffer, required_capacity);

    memcpy(buffer->ptr + buffer->length, buffer_to_append, length);
    buffer->length += length;
}

void mut_byte_buffer_append_str(
    struct MutableByteBuffer* buffer,
    const char* str)
{
    mut_byte_buffer_append_buffer(buffer, str, strlen(str) + 1);
}

void mut_byte_buffer_append_byte(struct MutableByteBuffer* buffer, uint8_t ch)
{
    mut_byte_buffer_append_buffer(buffer, &ch, 1);
}

const uint8_t* mut_byte_buffer_ptr(const struct MutableByteBuffer* buffer)
{
    return buffer->ptr;
}

size_t mut_byte_buffer_length(const struct MutableByteBuffer* buffer)
{
    return buffer->length;
}
