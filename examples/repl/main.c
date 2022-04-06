#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include <arpa/inet.h>
#include <netinet/in.h>

#include "simple_rpc/rpc_client.h"
#include "simple_rpc/rpc_value.h"
#include "util/byte_buffer.h"

void rpc_value_print(struct RPCValue value)
{
    switch (value.type) {
    case kRPCValueTypeNone:
        printf("<none>");
        break;
    case kRPCValueTypeInt:
        printf("%d", value.value_int);
        break;
    case kRPCValueTypeString:
        printf("\"%s\"", value.value_string);
        break;
    }
}

bool parse_query(struct RPCValue** out_values, size_t* out_count)
{
    const char* kWhitespace = " \f\n\r\t\v";
    const size_t kMaxQueryLength = 4096;
    char query[kMaxQueryLength];

    if (fgets(query, sizeof(query), stdin) == NULL) {
        return false;
    }

    char* new_line = strchr(query, '\n');
    if (new_line != NULL) {
        *new_line = '\0';
    }

    size_t count = 0;
    {
        char* begin = query;
        while (1) {
            begin += strspn(begin, kWhitespace);
            char* end = begin + strcspn(begin, kWhitespace);
            if (begin == end) {
                break;
            }

            ++count;

            begin = end;
        }
    }

    if (count == 0) {
        *out_values = NULL;
        *out_count = 0;
        return true;
    }

    *out_values = malloc(sizeof(struct RPCValue) * count);
    *out_count = count;

    {
        size_t index = 0;
        char* begin = query;
        while (1) {
            begin += strspn(begin, kWhitespace);
            char* end = begin + strcspn(begin, kWhitespace);
            if (begin == end) {
                break;
            }

            char* int_end;
            int value_int = strtol(begin, &int_end, 10);
            if (int_end == end) {
                (*out_values)[index] = rpc_int(value_int);
            } else {
                char* string = malloc(end - begin + 1);
                memcpy(string, begin, end - begin);
                string[end - begin] = '\0';
                (*out_values)[index] = rpc_allocated_string(string);
            }

            ++index;

            begin = end;
        }
    }

    return true;
}

void print_usage(const char* program_name)
{
    printf("Usage: %s [-h] [-p port]\n", program_name);
}

int main(int argc, char* argv[])
{
    const uint16_t kDefaultPort = 12000;
    uint16_t port = kDefaultPort;

    while (true) {
        int opt = getopt(argc, argv, "hp:");
        if (opt == -1) {
            break;
        }

        switch (opt) {
        case 'h':
            print_usage(argv[0]);
            return EXIT_SUCCESS;
        case 'p': {
            char* parsed_end;
            long parsed_port = strtol(optarg, &parsed_end, 10);
            if ((*parsed_end != '\0' && !isspace(*parsed_end)) ||
                parsed_port < 0 || parsed_port > UINT16_MAX) {
                printf(
                    "Error: specified port must be unsigned 16-bit integer.\n");
                return EXIT_FAILURE;
            }
            port = parsed_port;
            break;
        }
        default:
            return EXIT_FAILURE;
        }
    }

    struct RPCClient client;
    rpc_client_init_ipv4(&client, "127.0.0.1", port);

    bool should_stop = false;
    while (!should_stop) {
        printf(">>> ");
        fflush(stdout);

        struct RPCValue* values;
        size_t count;
        if (!parse_query(&values, &count)) {
            break;
        }

        if (values[0].type != kRPCValueTypeString) {
            printf("Name of procedure must be string\n");
        } else if (values[0].value_string[0] == '/') {
            if (strcmp(values[0].value_string, "/exit") == 0) {
                printf("Exiting...\n");
                should_stop = true;
            } else {
                printf("Unknown built-in command\n");
            }
        } else {
            struct RPCResult result = rpc_call_array(
                &client, values[0].value_string, values + 1, count - 1);

            if (result.is_error) {
                printf("Error: ");
                rpc_value_print(result.value);
                printf("\n");
            } else {
                printf("Result: ");
                rpc_value_print(result.value);
                printf("\n");
            }

            rpc_value_destroy(&result.value);
        }

        fflush(stdout);

        for (size_t i = 0; i < count; ++i) {
            rpc_value_destroy(&values[i]);
        }

        free(values);
    }

    rpc_client_destroy(&client);

    return 0;
}
