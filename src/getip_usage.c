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
print_usage(enum usage_variants variant)
{
    puts("GetIP (version " GETIP_VERSION ")\n"
          GETIP_DESCRIPTION "\n"
          GETIP_URL "\n"
    );

    if (variant != USAGE_APIS) {
        puts("Usage: getip [[targets][-]] [options]\n"
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
             "  -api-key <key>   Set API key\n"
             "  -raw             Print response in raw format\n"
        );
    } else if (variant == USAGE_FIELDS) {
        puts("List of supported field by current API:");
        if (check_field_support(API_CAP_IP)) {
            puts("  -ip                 Print IP field");
        } if (check_field_support(API_CAP_ORG)) {
            puts("  -org                Print ORG field");
        } if (check_field_support(API_CAP_HOST)) {
            puts("  -host               Print Hostname field");
        } if (check_field_support(API_CAP_AS)) {
            puts("  -as                 Print AS field");
        } if (check_field_support(API_CAP_AS_NAME)) {
            puts("  -asname             Print AS Name field");
        } if (check_field_support(API_CAP_ISP)) {
            puts("  -isp                Print ISP field");
        } if (check_field_support(API_CAP_CONTINENT)) {
            puts("  -continent          Print Continent field");
        } if (check_field_support(API_CAP_COUNTRY)) {
            puts("  -country            Print Country field");
        } if (check_field_support(API_CAP_REGION)) {
            puts("  -region             Print Region field");
        } if (check_field_support(API_CAP_CITY)) {
            puts("  -city               Print City field");
        } if (check_field_support(API_CAP_CITY)) {
            puts("  -zone               Print Time Zone field");
        } if (check_field_support(API_CAP_CITY)) {
            puts("  -is-host            Print Hosting field");
        } if (check_field_support(API_CAP_CITY)) {
            puts("  -is-proxy           Print Time Proxy field");
        } if (check_field_support(API_CAP_CITY)) {
            puts("  -is-mobile          Print Time Mobile field");
        }

        putchar('\n');
    } else if (variant == USAGE_APIS) {
        puts("List of supported apis:");
        for (int counter = 0; counter < APIS_COUNT; ++counter) {
            putchar(' '), putchar(' ');

            fputs(apis_list[counter].str_id, stdout);
            putchar(' ');
            if (apis_list[counter].can_use_key) {
                if (apis_list[counter].should_use_key) {
                    fputs("(REQ API KEY)", stdout);
                } else {
                    fputs("(API KEY)", stdout);
                }
            }

            if (!counter) {
                fputs("(Default)", stdout);
            }

            putchar('\n');
        }

        putchar('\n');
    }
}

