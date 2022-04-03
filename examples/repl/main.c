#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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

int main()
{
    struct RPCClient client;
    rpc_client_init_ipv4(&client, "127.0.0.1", 12000);

    while (1) {
        const char* kWhitespace = " \f\n\r\t\v";
        const size_t kMaxQueryLength = 4096;
        char query[kMaxQueryLength];

        printf(">>> ");
        fflush(stdout);

        if (fgets(query, sizeof(query), stdin) == NULL) {
            break;
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
            continue;
        }

        struct RPCValue values[count];

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
                    values[index] = rpc_int(value_int);
                } else {
                    char* string = malloc(end - begin + 1);
                    memcpy(string, begin, end - begin);
                    string[end - begin] = '\0';
                    values[index] = rpc_allocated_string(string);
                }

                ++index;

                begin = end;
            }
        }

        if (values[0].type != kRPCValueTypeString) {
            printf("Name of procedure must be string\n");
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
        }

        fflush(stdout);

        for (size_t i = 0; i < count; ++i) {
            rpc_value_destroy(&values[i]);
        }
    }

    rpc_client_destroy(&client);

    return 0;
}
