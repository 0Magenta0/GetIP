/*
 *  src/getip_errors.c
 *  This file is a part of the GetIP distribution released under GNU GPLv3
 *  Copyright (C) 2021-2023  0Magenta0
 *  https://github.com/0Magenta0/GetIP
 */

#include "getip_usage.h"
#include "getip_errors.h"

#include <stdio.h>
#include <stdbool.h>

/* Errors descriptions. */
#define ERR_STR_IP_STR            "invalid target paramenter"
#define ERR_STR_ARG_UNK           "unknown paramenter"
#define ERR_STR_ARG_MISS          "parameter should have argument"
#define ERR_STR_ARG_CANT_MMDB     "selected option doesn't support MMDB mode"
#define ERR_STR_ARG_API_UNK       "you're select invalid API"
#define ERR_STR_ARG_API_FIELD     "selected API doesn't support this field"
#define ERR_STR_ARG_API_NOKEY     "selected API doesn't support API keys"
#define ERR_STR_ARG_API_REQKEY    "selected API required API key"
#define ERR_STR_ARG_API_KEYLEN    "selected API key has an invalid lenght"
#define ERR_STR_ARG_API_NOTARGET  "selected API doesn't support TARGETS"
#define ERR_STR_ARG_API_REQTARGET "selected API required a TARGET"
#define ERR_STR_ARG_API_WHEN_MMDB "you can't use -api parameter with -mmdb"
#define ERR_STR_CURL_GLOB_INIT    "curl_global_init() failed"
#define ERR_STR_CURL_EASY_INIT    "curl_easy_init() failed"
#define ERR_STR_CURL_EASY_PERF    "curl_easy_perform() failed"
#define ERR_STR_API_RET_CODE      "API's response status code is not 200"
#define ERR_STR_API_JSON_PARSE    "cannot parse JSON API response"
#define ERR_STR_API_STATUS        "API response is not successful"
#define ERR_STR_API_RES_NULL          "API response body is empty"
#define ERR_STR_MMDB_OPEN         "MMDB open failure"
#define ERR_STR_MMDB_GAI          "MMDB can't resolve IP string"
#define ERR_STR_MMDB_REQUEST      "MMDB request failure"
#define ERR_STR_MMDB_TARGET       "MMDB require the TARGET"

enum error_id error_id;

void
error_handler(void)
{
    fputs("getip: ", stderr);

    switch (error_id) {
        case ERR_IP_STR:
            fputs(ERR_STR_IP_STR, stderr);
            putchar('\n');
            print_usage(USAGE_GENERAL);
            break;

        case ERR_ARG_MISS:
            fputs(ERR_STR_ARG_MISS, stderr);
            putchar('\n');
            print_usage(USAGE_GENERAL);
            break;

        case ERR_ARG_UNK:
            fputs(ERR_STR_ARG_UNK, stderr);
            putchar('\n');
            print_usage(USAGE_GENERAL);
            break;

        case ERR_ARG_CANT_MMDB:
            fputs(ERR_STR_ARG_CANT_MMDB, stderr);
            putchar('\n');
            break;

        case ERR_ARG_API_FIELD:
            fputs(ERR_STR_ARG_API_FIELD, stderr);
            putchar('\n');
            print_usage(USAGE_FIELDS);
            break;

        case ERR_ARG_API_NOKEY:
            fputs(ERR_STR_ARG_API_NOKEY, stderr);
            putchar('\n');
            break;

        case ERR_ARG_API_REQKEY:
            fputs(ERR_STR_ARG_API_REQKEY, stderr);
            putchar('\n');
            print_usage(USAGE_FIELDS);
            break;

        case ERR_ARG_API_KEYLEN:
            fputs(ERR_STR_ARG_API_KEYLEN, stderr);
            putchar('\n');
            break;

        case ERR_ARG_API_NOTARGET:
            fputs(ERR_STR_ARG_API_NOTARGET, stderr);
            putchar('\n');
            break;

        case ERR_ARG_API_REQTARGET:
            fputs(ERR_STR_ARG_API_REQTARGET, stderr);
            putchar('\n');
            print_usage(USAGE_FIELDS);
            break;

        case ERR_ARG_API_WHEN_MMDB:
            fputs(ERR_STR_ARG_API_WHEN_MMDB, stderr);
            putchar('\n');
            print_usage(USAGE_GENERAL);
            break;
        case ERR_CURL_GLOB_INIT:
            fputs(ERR_STR_CURL_GLOB_INIT, stderr);
            putchar('\n');
            break;

        case ERR_CURL_EASY_INIT:
            fputs(ERR_STR_CURL_EASY_INIT, stderr);
            putchar('\n');
            break;

        case ERR_CURL_EASY_PERF:
            fputs(ERR_STR_CURL_EASY_PERF, stderr);
            putchar('\n');
            break;

        case ERR_ARG_API_UNK:
            fputs(ERR_STR_ARG_API_UNK, stderr);
            putchar('\n');
            print_usage(USAGE_APIS);
            break;

        case ERR_API_RET_CODE:
            fputs(ERR_STR_API_RET_CODE, stderr);
            putchar('\n');
            break;

        case ERR_API_JSON_PARSE:
            fputs(ERR_STR_API_JSON_PARSE, stderr);
            putchar('\n');
            break;

        case ERR_API_STATUS:
            fputs(ERR_STR_API_STATUS, stderr);
            putchar('\n');
            break;

        case ERR_API_RES_NULL:
            fputs(ERR_STR_API_RES_NULL, stderr);
            putchar('\n');
            break;

        case ERR_MMDB_OPEN:
            fputs(ERR_STR_MMDB_OPEN, stderr);
            putchar('\n');
            break;

        case ERR_MMDB_GAI:
            fputs(ERR_STR_MMDB_GAI, stderr);
            putchar('\n');
            break;

        case ERR_MMDB_REQUEST:
            fputs(ERR_STR_MMDB_REQUEST, stderr);
            putchar('\n');
            break;

        case ERR_MMDB_TARGET:
            fputs(ERR_STR_MMDB_TARGET, stderr);
            putchar('\n');
            print_usage(USAGE_GENERAL);
            break;
    }
}

