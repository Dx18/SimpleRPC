#include "util/byte_buffer_io.h"

#include <unistd.h>

int mut_byte_buffer_read_until_eof(struct MutableByteBuffer* buffer, int fd)
{
    const size_t kBufferSize = 4096;
    uint8_t read_buffer[kBufferSize];

    while (1) {
        ssize_t bytes_read = read(fd, read_buffer, sizeof(read_buffer));
        if (bytes_read == -1) {
            return -1;
        }
        if (bytes_read == 0) {
            break;
        }

        mut_byte_buffer_append_buffer(buffer, read_buffer, bytes_read);
    }

    return 0;
}

int mut_byte_buffer_write(const struct MutableByteBuffer* buffer, int fd)
{
    size_t write_progress = 0;
    while (write_progress < buffer->length) {
        ssize_t bytes_written = write(
            fd, buffer->ptr + write_progress, buffer->length - write_progress);
        if (bytes_written == -1) {
            return -1;
        }
        write_progress += bytes_written;
    }

    return 0;
}
