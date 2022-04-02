function(simple_rpc_generate)
  add_custom_command(
    OUTPUT ${PROJECT_SOURCE_DIR}/rpc_generated/server.c
    COMMAND python3
    ${CMAKE_SOURCE_DIR}/scripts/generate_server.py
    ${PROJECT_SOURCE_DIR}
    DEPENDS rpc_info/)

  add_executable(${PROJECT_NAME}Server
    ${PROJECT_SOURCE_DIR}/rpc_generated/server.c)

  target_link_libraries(${PROJECT_NAME}Server UtilLib SimpleRPCLib)
endfunction()
