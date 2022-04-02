project(SimpleRPCLib)

set(SIMPLE_RPC_LIB_SOURCE
  src/simple_rpc/rpc_value.c
  src/simple_rpc/rpc_serialize.c
  src/simple_rpc/rpc_deserialize.c
  src/simple_rpc/rpc_server.c
  src/simple_rpc/rpc_client.c)

add_library(SimpleRPCLib STATIC ${SIMPLE_RPC_LIB_SOURCE})

target_include_directories(SimpleRPCLib PUBLIC include)

target_link_libraries(SimpleRPCLib UtilLib)
