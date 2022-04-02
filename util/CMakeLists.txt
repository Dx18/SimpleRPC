project(UtilLib)

set(UTIL_LIB_SOURCE
  src/util/byte_buffer.c
  src/util/byte_buffer_io.c)

add_library(UtilLib STATIC ${UTIL_LIB_SOURCE})

target_include_directories(UtilLib PUBLIC include)
