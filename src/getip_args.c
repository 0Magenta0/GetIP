/*
 *  src/getip_args.c
 *  This file is a part of the GetIP distribution released under GNU GPLv3
 *  Copyright (C) 2021-2023  0Magenta0
 *  https://github.com/0Magenta0/GetIP
 */

#include "getip_args.h"
#include "getip_errs.h"
#include "getip_apis.h"
#include "getip_usage.h"
#include "getip_request.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define MAX_IP_STR_LEN 254

bool is_verbose;
bool is_force;

bool
args_handler(const int argc, char * const argv[]) {
    size_t ip_str_len;

    if (argc == 0) {
        return false;
    } else if (argc == 1) {
        return true;
    } else {
        ip_str_len = strlen(argv[1]);
        if (!ip_str_len || ip_str_len > MAX_IP_STR_LEN) {
            errs_status = ERRS_ARGS_IP_STR_LEN;
            return false;
        } if (argv[1][0] != '-') {
            is_external_ip = true;
        }

        for (int counter = (is_external_ip) ? 2 : 1; counter < argc; ++counter) {
            if (!strcmp(argv[counter], "-verbose")) {
                is_verbose = true;
                continue;
            } if (!strcmp(argv[counter], "-force")) {
                is_force = true;
                continue;
            }
        }

        for (int counter = (is_external_ip) ? 2 : 1; counter < argc; ++counter) {
            if (!strcmp(argv[counter], "-help")) {
                print_usage(USAGE_GENERAL);
                exit(EXIT_SUCCESS);
            } if (!strcmp(argv[counter], "-api-list")) {
                print_usage(USAGE_APIS);
                exit(EXIT_SUCCESS);
            }
        }

        for (int counter = (is_external_ip) ? 2 : 1; counter < argc; ++counter) {
            if (!strcmp(argv[counter], "-api")) {
                if (argv[++counter] == NULL) {
                    errs_status = ERRS_ARGS_API_NULL;
                    return false;
                }

                if (is_verbose) {
                    fprintf(stderr, "Trying to select API: %s\n", argv[counter]);
                }

                if (!select_api_by_str_id(argv[counter])) {
                    if (is_force) {
                        puts("getip: Cannot select API, selecting default...");
                    } else {
                        errs_status = ERRS_ARGS_API_UNK;
                        return false;
                    }
                }
            }
        }

        for (int counter = (is_external_ip) ? 2 : 1; counter < argc; ++counter) {
            if (!strcmp(argv[counter], "-fields-list")) {
                print_usage(USAGE_FIELDS);
                exit(EXIT_SUCCESS);
            }
        }

        for (int counter = (is_external_ip) ? 2 : 1; counter < argc; ++counter) {
            if (!strcmp(argv[counter], "-i")) {
                if (check_field_support(API_IP)) {

                } else {
                    errs_status = ERRS_ARGS_FIELD_SUPPORT;
                    return false;
                }

                continue;
            } if (!strcmp(argv[counter], "-H")) {

                continue;
            } if (!strcmp(argv[counter], "-c")) {

                continue;
            } if (!strcmp(argv[counter], "-r")) {
                if (check_field_support(API_REGION)) {

                } else {
                    errs_status = ERRS_ARGS_FIELD_SUPPORT;
                    return false;
                }

                continue;
            }
        }

        return true;
    }
}

