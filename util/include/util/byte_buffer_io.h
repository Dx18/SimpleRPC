#pragma once

#include "util/byte_buffer.h"

/**
 * Reads all contents of given file descriptor `fd` to given
 * buffer. Returns -1 if IO error occurred (actual error code is stored in
 * `errno`). Returns 0 on success. Does not restore buffer to previous
 * state in case of failure.
 */
int mut_byte_buffer_read_until_eof(struct MutableByteBuffer* buffer, int fd);

/**
 * Writes all contents of given buffer to file descriptor
 * `fd`. Returns -1 if IO error occurred (actual error code is stored
 * in `errno`). Returns 0 on success.
 */
int mut_byte_buffer_write(const struct MutableByteBuffer* buffer, int fd);
