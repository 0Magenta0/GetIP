/*
 *  src/getip_args.c
 *  This file is a part of the GetIP distribution released under GNU GPLv3
 *  Copyright (C) 2021-2023  0Magenta0
 *  https://github.com/0Magenta0/GetIP
 */

#include "getip_args.h"
#include "getip_usage.h"
#include "getip_errors.h"

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <stdnoreturn.h>

#define MIN_IP_STR_LEN   3
#define MAX_IP_STR_LEN 254

#define MAX_OPT_PRIORITY 1
#define OPTIONS_COUNT (sizeof (options_list) / sizeof (struct getip_option))

enum getip_option_type {
    GETIP_OPTION_ARG,
    GETIP_OPTION_NO_ARG
};

struct getip_option {
    const char * const option_name;
    const enum getip_option_type option_type;
    bool (* const func)(char *);
    const int option_priority;
};

bool
ip_string_validate(const char * const ip_str);

_Noreturn bool
help_opt(char *);

bool
verbose_opt(char *);

bool is_verbose;

struct getip_option options_list[] = {
    { "help",
      GETIP_OPTION_NO_ARG,
      help_opt,
      0
    },

    { "verbose",
      GETIP_OPTION_NO_ARG,
      verbose_opt,
      0
    },
};

bool
args_handler(const int    argc,
             char * const argv[])
{
    if (argc == 1 || !argc) {
        /* is_external_ip = false */
        return true;
    } else {
        if (argv[1][0] != '-' && !ip_string_validate(argv[1])) {
            error_id = ERR_IP_STR;
            return false;
        }

        /* TODO: Validate args */

        for (int priority = 0; priority <= MAX_OPT_PRIORITY; ++priority) {
            for (int arg_counter = 1; arg_counter < argc; ++arg_counter) {
                for (int opt_counter = 0; opt_counter < (int) OPTIONS_COUNT; ++opt_counter) {
                    if (options_list[opt_counter].option_priority != priority) {
                        continue;
                    }

                    if (!strcmp(options_list[opt_counter].option_name, argv[arg_counter] + 1)) {
                        if (options_list[opt_counter].option_type == GETIP_OPTION_ARG) {
                            options_list[opt_counter].func(argv[++arg_counter]);
                        } else {
                            options_list[opt_counter].func(NULL);
                        }

                        goto _opt_found;
                    }
                }

_opt_found:
                continue;
            }
        }

        return true;
    }
}

bool
ip_string_validate(const char * const ip_str)
{
    register size_t len;

    if ((len = strlen(ip_str)) < MIN_IP_STR_LEN || len > MAX_IP_STR_LEN) {
        return false;
    }

    for (size_t counter = 0; ip_str[counter]; ++counter) {
        if (isblank(ip_str[counter])) {
            return false;
        }
    }

    return true;
}

noreturn bool
help_opt(char *n)
{
    (void) n;

    print_usage(USAGE_GENERAL);
    exit(EXIT_SUCCESS);
}

bool
verbose_opt(char *n)
{
    (void) n;

    return (is_verbose = true);
}

