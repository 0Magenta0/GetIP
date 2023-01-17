/*
 *  src/getip_args.c
 *  This file is a part of the GetIP distribution released under GNU GPLv3
 *  Copyright (C) 2021-2023  0Magenta0
 *  https://github.com/0Magenta0/GetIP
 */

#include "getip_args.h"
#include "getip_errs.h"

#include <stdio.h>
#include <string.h>
#include <stdbool.h>

#define MAX_IP_STR_LEN 254

bool
args_handler(const int argc, char * const argv[]) {
    int counter = 1; /* Skip the argv[0] */
    size_t ip_str_len;

    if (argc == 0) {
        return false;
    } else if (argc == 1) {
        /* external_ip = false */
        return true;
    } else {
        ip_str_len = strlen(argv[1]);
        if (!ip_str_len || ip_str_len > MAX_IP_STR_LEN) {
            errs_status = ERRS_ARGS_IP_STR_LEN;
            return false;
        } if (argv[1][0] != '-') {
            /* external_ip = true */
            ++counter;
        }

        for (; counter < argc; ++counter) {
            puts(argv[counter]);
        }

        return true;
    }
}

