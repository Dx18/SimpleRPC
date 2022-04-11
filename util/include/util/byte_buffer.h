#pragma once

#include <stdint.h>
#include <stdlib.h>

/** Min positive capacity of mutable byte buffer. */
extern const size_t kByteBufferMinPositiveCapacity;

/**
 * Auto-resizable mutable byte buffer. Empty buffer is not allocated.
 */
struct MutableByteBuffer {
    /**
     * Beginning of buffer (or `NULL` if buffer is not yet allocated).
     */
    uint8_t* ptr;
    /** Number of bytes can be stored in buffer. */
    size_t capacity;
    /** Number of bytes stored in buffer. */
    size_t length;
};

/** Initializes empty byte buffer. Does not make allocations. */
void mut_byte_buffer_init(struct MutableByteBuffer* buffer);

/** Destroys byte buffer by freeing underlying memory if presented. */
void mut_byte_buffer_destroy(struct MutableByteBuffer* buffer);

/**
 * Ensures that capacity of given byte buffer is at least
 * `required_capacity`. May make allocations.
 */
void mut_byte_buffer_extend_reallocate(
    struct MutableByteBuffer* buffer,
    size_t required_capacity);

/**
 * Appends `length` bytes from address `buffer_to_append` to given
 * buffer.
 */
void mut_byte_buffer_append_buffer(
    struct MutableByteBuffer* buffer,
    const void* buffer_to_append,
    size_t length);

/**
 * Appends C-string `str` to given buffer (including trailing '\0').
 */
void mut_byte_buffer_append_str(
    struct MutableByteBuffer* buffer,
    const char* str);

/** Appends single byte to given buffer. */
void mut_byte_buffer_append_byte(
    struct MutableByteBuffer* buffer,
    uint8_t byte);

/**
 * Returns const pointer to memory allocated by buffer (may return
 * `NULL` if buffer is not allocated).
 */
const uint8_t* mut_byte_buffer_ptr(const struct MutableByteBuffer* buffer);

/** Returns number of bytes stored in given buffer. */
size_t mut_byte_buffer_length(const struct MutableByteBuffer* buffer);
