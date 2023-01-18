/*
 *  src/getip_usage.c
 *  This file is a part of the GetIP distribution released under GNU GPLv3
 *  Copyright (C) 2021-2023  0Magenta0
 *  https://github.com/0Magenta0/GetIP
 */

#include "getip_apis.h"
#include "getip_usage.h"

#include <stdio.h>

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
             "  -force           Ignore warnings\n"
             "  -verbose         Debug output to stderr\n"
             "  -api <str>       Select API\n"
             "  -api-list        List of supported apis\n"
             "  -fields-list     List of supported fields by selected API\n"
        );
    } else if (variant == USAGE_FIELDS) {
        if (check_field_support(API_IP)) {

        }

        puts("  -ip              Print IP field\n"
             "  -domain          Print HOSTNAME field\n"
             "  -country         Print COUNTRY field\n"
             "  -region          Print REGION field\n"
        );
    } else if (variant == USAGE_APIS) {
        puts("List of supported apis:");
        for (int counter = 0; counter < APIS_COUNT; ++counter) {
            putchar(' '), putchar(' ');

            if (supported_apis[counter].is_default) {
                printf("%s (Default)\n", supported_apis[counter].id_name.api_name);
            } else {
                puts(supported_apis[counter].id_name.api_name);
            }
        }
    }
}

