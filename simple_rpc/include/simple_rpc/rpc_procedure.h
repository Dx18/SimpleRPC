#pragma once

#include "util/byte_buffer.h"

struct RPCProcedure {
    const char* name;
    struct RPCValue (*caller)(const struct MutableByteBuffer*, size_t*);
};
