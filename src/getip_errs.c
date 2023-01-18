/*
 *  src/getip_errs.c
 *  This file is a part of the GetIP distribution released under GNU GPLv3
 *  Copyright (C) 2021-2023  0Magenta0
 *  https://github.com/0Magenta0/GetIP
 */

#include "getip_errs.h"
#include "getip_usage.h"

#include <stdio.h>

#define ERRS_ESTR_UNK                "Unknown error"
#define ERRS_ESTR_ARGS_COUNT         "Have no arguments passed"
#define ERRS_ESTR_ARGS_IP_STR_LEN    "IP paramenter cannot be NULL-lenght or too long"
#define ERRS_ESTR_ARGS_FIELD_SUPPORT "Selected API doesn't support this field (Use: -force flag to ignore)"
#define ERRS_ESTR_ARGS_API_NULL      "Selected API argument is NULL"
#define ERRS_ESTR_ARGS_API_UNK       "Selected API is unknown"
#define ERRS_ESTR_CURL_GLOB_INIT     "Cannot initialize a curl library"
#define ERRS_ESTR_CURL_EASY_INIT     "Cannot initialize a curl request"
#define ERRS_ESTR_CURL_EASY_PERF     "Cannot realize a curl request"

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

        case ERRS_ARGS_FIELD_SUPPORT:
            fputs("getip: " ERRS_ESTR_ARGS_FIELD_SUPPORT "\n", stderr);
            break;

        case ERRS_ARGS_API_NULL:
            fputs("getip: " ERRS_ESTR_ARGS_API_NULL "\n", stderr);
            break;

        case ERRS_ARGS_API_UNK:
            fputs("getip: " ERRS_ESTR_ARGS_API_UNK "\n", stderr);
            break;

        case ERRS_CURL_GLOB_INIT:
            fputs("getip: " ERRS_ESTR_CURL_GLOB_INIT "\n", stderr);
            break;

        case ERRS_CURL_EASY_INIT:
            fputs("getip: " ERRS_ESTR_CURL_EASY_INIT "\n", stderr);
            break;

        case ERRS_CURL_EASY_PERF:
            fputs("getip: " ERRS_ESTR_CURL_EASY_PERF "\n", stderr);
            break;

        default:
            fputs("getip: " ERRS_ESTR_UNK "\n", stderr);
    }

    putchar('\n');
    print_usage(USAGE_GENERAL);
}

