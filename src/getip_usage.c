/*
 *  src/getip_usage.c
 *  This file is a part of the GetIP distribution released under GNU GPLv3
 *  Copyright (C) 2021-2023  0Magenta0
 *  https://github.com/0Magenta0/GetIP
 */

#include "getip_apis.h"
#include "getip_usage.h"
#include "getip_request.h"

#include <stdio.h>
#include <stdlib.h>

void
print_usage(enum usage_variants variant) {
    puts("GetIP (version " GETIP_VERSION ")\n"
          GETIP_DESCRIPTION "\n"
          GETIP_URL "\n"
    );

    if (variant != USAGE_APIS) {
        puts("Usage: getip [target] [options]\n"
             " or:   getip [options]\n"
        );
    }

    if (variant == USAGE_GENERAL) {
        puts("  -help            Print this message and exit\n"
             "  -verbose         Debug output to stderr\n"
             "  -api <api-id>    Select API\n"
             "  -api-list        List of supported apis\n"
             "  -fields-list     List of supported fields by selected API\n"
             "  -agent <ua>      Custom User-Agent header\n"
        );
    } else if (variant == USAGE_FIELDS) {
        puts("List of supported field by current API:");
        if (check_field_support(API_CAP_IP)) {
            puts("  -ip              Print IP field");
        } if (check_field_support(API_CAP_HOST)) {
            puts("  -domain          Print HOSTNAME field");
        } if (check_field_support(API_CAP_COUNTRY)) {
            puts("  -country         Print COUNTRY field");
        }

        putchar('\n');
    } else if (variant == USAGE_APIS) {
        puts("List of supported apis:");
        for (int counter = 0; counter < APIS_COUNT; ++counter) {
            putchar(' '), putchar(' ');

            if (!counter) {
                printf("%s (Default)\n", apis_list[counter].str_id);
            } else {
                puts(apis_list[counter].str_id);
            }
        }

        putchar('\n');
    }
}

