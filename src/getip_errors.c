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

#define ERR_STR_IP_STR   "invalid target paramenter"
#define ERR_STR_ARG_UNK  "unknown paramenter"
#define ERR_STR_ARG_MISS "parameter should have argument"

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

        case ERR_ARG_UNK:
            fputs(ERR_STR_ARG_UNK, stderr);
            putchar('\n');
            print_usage(USAGE_GENERAL);
            break;

        case ERR_ARG_MISS:
            fputs(ERR_STR_ARG_MISS, stderr);
            putchar('\n');
            print_usage(USAGE_GENERAL);
            break;
    }
}

