#pragma once

#include <stdint.h>
#include <stdlib.h>

extern const size_t kByteBufferMinPositiveCapacity;

struct MutableByteBuffer {
    uint8_t* ptr;
    size_t capacity;
    size_t length;
};

void mut_byte_buffer_init(struct MutableByteBuffer* buffer);

void mut_byte_buffer_destroy(struct MutableByteBuffer* buffer);

void mut_byte_buffer_extend_reallocate(
    struct MutableByteBuffer* buffer,
    size_t required_capacity);

void mut_byte_buffer_append_buffer(
    struct MutableByteBuffer* buffer,
    const void* buffer_to_append,
    size_t length);

void mut_byte_buffer_append_str(
    struct MutableByteBuffer* buffer,
    const char* str);

void mut_byte_buffer_append_byte(
    struct MutableByteBuffer* buffer,
    uint8_t byte);

const uint8_t* mut_byte_buffer_ptr(const struct MutableByteBuffer* buffer);

size_t mut_byte_buffer_length(const struct MutableByteBuffer* buffer);
