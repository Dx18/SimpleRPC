import sys
import os
import json

TYPE_ENUM_NAMES = {"int": "kRPCValueTypeInt",
                   "string": "kRPCValueTypeString"}
TYPE_C_NAMES = {"int": "int",
                "string": "const char*"}

assert len(sys.argv) >= 2, "At least one argument is required"

path = sys.argv[1]
input_path = os.path.join(path, "rpc_info")
output_path = os.path.join(path, "rpc_generated")

os.makedirs(output_path, exist_ok=True)

functions = []
headers = []

for path in os.listdir(input_path):
    full_path = os.path.join(input_path, path)
    name, extension = os.path.splitext(path)
    if not os.path.isfile(full_path) or extension != ".c":
        continue

    with open(full_path) as source_file:
        source = source_file.read()

    with open(full_path + ".json", "r") as data_file:
        data = json.loads(data_file.read())

    header_file_name = os.path.join(output_path, name + ".h")

    with open(header_file_name, "w") as header_file:
        headers.append(name + ".h")

        header_file.write(
"""#pragma once

#include "simple_rpc/rpc_deserialize.h"
#include "simple_rpc/rpc_procedure.h"
#include "simple_rpc/rpc_server.h"
#include "simple_rpc/rpc_value.h"
#include "util/byte_buffer.h"

""")

        header_file.write(source)

        for function in data:
            functions.append(function)
            
            function_name = function["function_name"]
            params = function["params"]
            result = function["result"]
            header_file.write(
f"""
struct RPCValue
{function_name}_caller(const struct MutableByteBuffer* buffer, size_t* index)
{{
    const size_t kArgCount = {len(params)};
    struct RPCValue args[kArgCount];
    for (size_t i = 0; i < kArgCount; ++i) {{
        if (rpc_value_deserialize(buffer, index, &args[i]) !=
            kRPCDeserializeResultOk) {{
            for (size_t j = 0; j < i; ++j) {{
                rpc_value_destroy(&args[j]);
            }}
            return rpc_string("Could not deserialize all arguments");
        }}
    }}
""")
            for index, arg_type in enumerate(params):
                arg_type_enum_name = TYPE_ENUM_NAMES[arg_type]
                arg_type_c_name = TYPE_C_NAMES[arg_type]
                header_file.write(
f"""
    if (args[{index}].type != {arg_type_enum_name}) {{
        for (size_t i = 0; i < kArgCount; ++i) {{
            rpc_value_destroy(&args[i]);
        }}
        return rpc_string("Argument #{index} must be `{arg_type}`");
    }}
    {arg_type_c_name} arg_{index} = args[{index}].value_{arg_type};
""")
            header_file.write(
f"""
    struct RPCValue result = rpc_{result}({function_name}({", ".join([f"arg_{i}" for i in range(len(params))])}));

    for (size_t i = 0; i < kArgCount; ++i) {{
        rpc_value_destroy(&args[i]);
    }}

    return result;
}}
""")

with open(os.path.join(output_path, "server.c"), "w") as server_file:
    server_file.write(
"""#include <arpa/inet.h>
#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <unistd.h>

#include "simple_rpc/rpc_server.h"

""")

    for header in headers:
        server_file.write(f"#include \"{header}\"\n")

    server_file.write(
f"""
const size_t kProcedureCount = {len(functions)};
const struct RPCProcedure kProcedures[] = {{
""")

    for function in functions:
        function_name = function["function_name"]
        server_file.write(f"    {{.name = \"{function_name}\", .caller = {function_name}_caller}},\n")

    server_file.write(
f"""}};

void print_usage(const char *program_name)
{{
    printf("Usage: %s [-h] [-p port]\\n", program_name);
}}

int main(int argc, char *argv[])
{{
    const uint16_t kDefaultPort = 12000;
    uint16_t port = kDefaultPort;

    while (true) {{
        int opt = getopt(argc, argv, "hp:");
        if (opt == -1) {{
            break;
        }}

        switch (opt) {{
        case 'h':
            print_usage(argv[0]);
            return EXIT_SUCCESS;
        case 'p': {{
            char* parsed_end;
            long parsed_port = strtol(optarg, &parsed_end, 10);
            if ((*parsed_end != '\\0' && !isspace(*parsed_end)) ||
                parsed_port < 0 || parsed_port > UINT16_MAX) {{
                printf(
                    "Error: specified port must be unsigned 16-bit integer.\\n");
                return EXIT_FAILURE;
            }}
            port = parsed_port;
            break;
        }}
        default:
            return EXIT_FAILURE;
        }}
    }}

    struct sockaddr_in addr = {{
        .sin_family = AF_INET,
        .sin_addr = {{.s_addr = inet_addr("0.0.0.0")}},
        .sin_port = htons(port),
    }};

    printf("Starting server at port %u...\\n", port);

    int result = run_server(
        (const struct sockaddr*)&addr,
        sizeof(addr),
        kProcedures,
        kProcedureCount);

    if (result != EXIT_SUCCESS) {{
        perror("An error occurred while running a server");
    }}

    return result;
}}
""")
