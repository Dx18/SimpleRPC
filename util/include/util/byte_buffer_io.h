#pragma once

#include "util/byte_buffer.h"

int mut_byte_buffer_read_until_eof(struct MutableByteBuffer* buffer, int fd);

int mut_byte_buffer_write(const struct MutableByteBuffer* buffer, int fd);
