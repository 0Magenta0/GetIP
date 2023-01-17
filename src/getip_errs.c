/*
 *  src/getip_errs.c
 *  This file is a part of the GetIP distribution released under GNU GPLv3
 *  Copyright (C) 2021-2023  0Magenta0
 *  https://github.com/0Magenta0/GetIP
 */

#include "getip_errs.h"
#include "getip_usage.h"

#include <stdio.h>

#define ERRS_ESTR_UNK             "Unknown error"
#define ERRS_ESTR_ARGS_COUNT      "Have no arguments passed"
#define ERRS_ESTR_ARGS_IP_STR_LEN "IP paramenter cannot be NULL-lenght or too long"

enum errs_status errs_status;

void
errs_handler(void) {
    switch (errs_status) {
        case ERRS_ARGS_COUNT:
            fputs("getip: " ERRS_ESTR_ARGS_COUNT "\n", stderr);
            break;

        case ERRS_ARGS_IP_STR_LEN:
            fputs("getip: " ERRS_ESTR_ARGS_IP_STR_LEN "\n", stderr);
            break;

        default:
            fputs("getip: " ERRS_ESTR_UNK "\n", stderr);
    }

    putchar('\n');
    print_usage();
}

