project(REPL)

add_executable(REPL main.c)

target_link_libraries(REPL UtilLib SimpleRPCLib)
