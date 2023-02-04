/*
 *  src/getip_args.c
 *  This file is a part of the GetIP distribution released under GNU GPLv3
 *  Copyright (C) 2021-2023  0Magenta0
 *  https://github.com/0Magenta0/GetIP
 */

#include "getip_args.h"
#include "getip_apis.h"
#include "getip_usage.h"
#include "getip_errors.h"
#include "getip_request.h"

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <stdnoreturn.h>

#define MAX_OPT_PRIORITY 3
#define OPTIONS_COUNT (sizeof (options_list) / sizeof (struct getip_option))

/* Terminal parameter types
 * enumerator
 */
enum getip_option_type {
    GETIP_OPTION_ARG,         /* Parameter required argument.   */
    GETIP_OPTION_NO_ARG,      /* Parameter have no arguments.   */
    GETIP_OPTION_ARG_HAVE_EMP /* Parameter required argument 
                                 that can contains blank chars. */
};

/* Struct that defines
 * GetIP's parameter structure
 */
struct getip_option {
    const char * const option_name;
    const enum getip_option_type option_type;
    bool (* const func)(char *);
    const int option_priority; /* Less number - more priority. */
};

/* Verify target string
 * lenght and blank symbols.
 */
bool
ip_string_validate(const char *ip_str);

/* Check if string doesn't
 * contains a blank characters.
 */
bool
string_have_no_empty(const char *str);

/* Check if string contains a
 * non-blank characters.
 */
bool
string_have_no_full_empty(const char * str);

/* Allocates new target structure */
void
append_ip_str(size_t ip_str_len);

noreturn bool
help_opt(char *);

bool
verbose_opt(char *);

noreturn bool
api_list_opt(char *);

bool
api_opt(char *api_str_id);

noreturn bool
fileds_list_opt(char *);

bool
agent_opt(char *agent);

bool
raw_opt(char *);

bool
no_delim_opt(char *);

bool
ip_opt(char *);

bool
org_opt(char *);

bool
host_opt(char *);

bool
as_opt(char *);

bool
asname_opt(char *);

bool
isp_opt(char *);

bool
continent_opt(char *);

bool
country_opt(char *);

bool
region_opt(char *);

bool
city_opt(char *);

bool
zone_opt(char *);

bool
is_host_opt(char *);

bool
is_proxy_opt(char *);

bool
is_mobile_opt(char *);

bool is_verbose;
bool is_raw;
bool is_no_delim;

const struct getip_option options_list[] = {
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

    { "api-list",
      GETIP_OPTION_NO_ARG,
      api_list_opt,
      0
    },

    { "api",
      GETIP_OPTION_ARG,
      api_opt,
      1
    },

    { "fields-list",
      GETIP_OPTION_NO_ARG,
      fileds_list_opt,
      2
    },

    { "agent",
      GETIP_OPTION_ARG_HAVE_EMP,
      agent_opt,
      2
    },

    { "raw",
      GETIP_OPTION_NO_ARG,
      raw_opt,
      2
    },

    { "no-delim",
      GETIP_OPTION_NO_ARG,
      no_delim_opt,
      2
    },

    { "ip",
      GETIP_OPTION_NO_ARG,
      ip_opt,
      3
    },

    { "org",
      GETIP_OPTION_NO_ARG,
      org_opt,
      3
    },

    { "host",
      GETIP_OPTION_NO_ARG,
      host_opt,
      3
    },

    { "as",
      GETIP_OPTION_NO_ARG,
      as_opt,
      3
    },

    { "asname",
      GETIP_OPTION_NO_ARG,
      asname_opt,
      3
    },

    { "isp",
      GETIP_OPTION_NO_ARG,
      isp_opt,
      3
    },

    { "continent",
      GETIP_OPTION_NO_ARG,
      continent_opt,
      3
    },

    { "country",
      GETIP_OPTION_NO_ARG,
      country_opt,
      3
    },

    { "region",
      GETIP_OPTION_NO_ARG,
      region_opt,
      3
    },

    { "city",
      GETIP_OPTION_NO_ARG,
      city_opt,
      3
    },

    { "zone",
      GETIP_OPTION_NO_ARG,
      zone_opt,
      3
    },

    { "is-host",
      GETIP_OPTION_NO_ARG,
      is_host_opt,
      3
    },

    { "is-proxy",
      GETIP_OPTION_NO_ARG,
      is_proxy_opt,
      3
    },

    { "is-mobile",
      GETIP_OPTION_NO_ARG,
      is_mobile_opt,
      3
    },
};

bool
args_handler(const int    argc,
             char * const argv[])
{
    size_t ip_str_len;
    bool is_next_ip = true;
    int ips_count = 0;
    int ch;

    if (argc == 1 || !argc) {
        /* Select default API when
         * have no arguments.
         */
        selected_capabilites =
            get_api_by_id(selected_api)->capabilities;
        return true;
    } else {
        if (argv[1][0] != '-') {
            /* While arguments contains
             * a targets strings.
             */
            while (ips_count + 1 < argc && argv[ips_count + 1][0] != '-') {
                if (!ip_string_validate(argv[ips_count + 1])) {
                    error_id = ERR_IP_STR;
                    return false;
                }

                ip_str_len = strlen(argv[ips_count + 1]);
                append_ip_str(ip_str_len);
                strncpy(last_external_ip->ip_str, argv[ips_count + 1], ip_str_len);
                last_external_ip->ip_str[ip_str_len] = '\0';
                last_external_ip->str_len = ip_str_len;
                ++ips_count;
            }

            is_external_ips = true;
        } else if (argv[1][0] == '-' && argv[1][1] == '\0') {
            /* While use `stdin` and non-EOF. */
            while ((ch = getchar()) != EOF) {
                if (is_next_ip) {
                    if (ch == '\n') {
                        error_id = ERR_IP_STR;
                        return false;
                    } else {
                        /* Allocate new target
                         * structure when is needed.
                         */
                        append_ip_str(MAX_IP_STR_LEN);
                        is_next_ip = false;
                        ip_str_len = 0;
                    }
                }

                /* Verify input and handle
                 * characters from `stdin`.
                 */
                if (ip_str_len < MAX_IP_STR_LEN) {
                    if (ch == '\n') {
                        if (ip_str_len < MIN_IP_STR_LEN) {
                            error_id = ERR_IP_STR;
                            return false;
                        }

                        is_next_ip = true;
                        last_external_ip->ip_str[ip_str_len] = '\0';
                        last_external_ip->str_len = ip_str_len;
                        continue;
                    }

                    if (isblank(ch)) {
                        error_id = ERR_IP_STR;
                        return false;
                    }

                    last_external_ip->ip_str[ip_str_len] = (char) ch;
                    ++ip_str_len;
                }
            }

            /* Append Zero-terminator at end
             * in case the input is too long.
             */
            last_external_ip->ip_str[ip_str_len] = '\0';
            is_external_ips = true;
            ++ips_count;
        }

        /* All parameters validation. */
        for (int arg_counter = ips_count + 1; arg_counter < argc; ++arg_counter) {
            for (int opt_counter = 0; opt_counter < (int) OPTIONS_COUNT; ++opt_counter) {
                if (!strcmp(options_list[opt_counter].option_name, argv[arg_counter] + 1)) {
                    if (options_list[opt_counter].option_type == GETIP_OPTION_ARG ||
                        options_list[opt_counter].option_type == GETIP_OPTION_ARG_HAVE_EMP) {
                        if (++arg_counter >= argc) {
                            error_id = ERR_ARG_MISS;
                            return false;
                        }
                    }

                    if (options_list[opt_counter].option_type == GETIP_OPTION_ARG) {
                        if (!string_have_no_empty(argv[arg_counter])) {
                            error_id = ERR_ARG_MISS;
                            return false;
                        }
                    }

                    if (options_list[opt_counter].option_type == GETIP_OPTION_ARG_HAVE_EMP) {
                        if (!string_have_no_full_empty(argv[arg_counter])) {
                            error_id = ERR_ARG_MISS;
                            return false;
                        }
                    }

                    goto _valid_arg;
                }
            }

            error_id = ERR_ARG_UNK;
            return false;

_valid_arg:
            continue;
        }

        /* If args successfuly validated handle it. */
        for (int priority = 0; priority <= MAX_OPT_PRIORITY; ++priority) {
            for (int arg_counter = ips_count + 1; arg_counter < argc; ++arg_counter) {
                for (int opt_counter = 0; opt_counter < (int) OPTIONS_COUNT; ++opt_counter) {
                    /* Continue loop if parameters
                     * priority is not same.
                     */
                    if (options_list[opt_counter].option_priority != priority) {
                        continue;
                    }

                    if (!strcmp(options_list[opt_counter].option_name, argv[arg_counter] + 1)) {
                        if (options_list[opt_counter].option_type == GETIP_OPTION_ARG ||
                            options_list[opt_counter].option_type == GETIP_OPTION_ARG_HAVE_EMP) {
                            if (!options_list[opt_counter].func(argv[++arg_counter])) {
                                return false;
                            }
                        } else {
                            if (!options_list[opt_counter].func(NULL)) {
                                return false;
                            }
                        }

                        goto _opt_found;
                    }
                }

_opt_found:
                continue;
            }
        }

        /* If no one API parameter is
         * selected then use ALL.
         */
        if (!selected_capabilites) {
            selected_capabilites =
                get_api_by_id(selected_api)->capabilities;
        }

        return true;
    }
}

bool
ip_string_validate(const char * ip_str)
{
    size_t len;

    if ((len = strlen(ip_str)) < MIN_IP_STR_LEN || len > MAX_IP_STR_LEN) {
        return false;
    }

    for (; *ip_str; ++ip_str) {
        if (isblank(*ip_str)) {
            return false;
        }
    }

    return true;
}

bool
string_have_no_empty(const char * str)
{
    if (!str[0]) {
        return false;
    }

    for (; *str; ++str) {
        if (isblank(*str)) {
            return false;
        }
    }

    return true;
}

bool
string_have_no_full_empty(const char *str)
{
    if (!str[0]) {
        return false;
    }

    for (; *str; ++str) {
        if (!isblank(*str)) {
            return true;
        }
    }

    return false;
}

void
append_ip_str(size_t ip_str_len)
{
    if (last_external_ip == NULL) {
        external_ip = malloc(sizeof (struct external_ip));
        memset(external_ip, 0, sizeof (struct external_ip));
        last_external_ip = external_ip;
        last_external_ip->ip_str = malloc(ip_str_len + 1);
    } else {
        last_external_ip->next = malloc(sizeof (struct external_ip));
        last_external_ip = last_external_ip->next;
        memset(last_external_ip, 0, sizeof (struct external_ip));
        last_external_ip->ip_str = malloc(ip_str_len + 1);
    }
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

noreturn bool
api_list_opt(char *n) {
    (void) n;

    print_usage(USAGE_APIS);
    exit(EXIT_SUCCESS);
}

bool
api_opt(char *api_str_id)
{
    if (!select_api_by_str_id(api_str_id)) {
        error_id = ERR_ARG_API_UNK;
        return false;
    }

    return true;
}

noreturn bool
fileds_list_opt(char *n)
{
    (void) n;

    print_usage(USAGE_FIELDS);
    exit(EXIT_SUCCESS);
}

bool
agent_opt(char *agent)
{
    is_custom_agent = true;
    custom_agent = agent;

    return true;
}

bool
raw_opt(char *n)
{
    (void) n;

    return (is_raw = true);
}

bool
no_delim_opt(char *n)
{
    (void) n;

    return (is_no_delim = true);
}

bool
ip_opt(char *n)
{
    (void) n;

    if (check_field_support(API_CAP_IP)) {
        selected_capabilites |= API_CAP_IP;
        return true;
    }

    error_id = ERR_ARG_API_FIELD;
    return false;
}

bool
org_opt(char *n)
{
    (void) n;

    if (check_field_support(API_CAP_ORG)) {
        selected_capabilites |= API_CAP_ORG;
        return true;
    }

    error_id = ERR_ARG_API_FIELD;
    return false;
}

bool
host_opt(char *n)
{
    (void) n;

    if (check_field_support(API_CAP_HOST)) {
        selected_capabilites |= API_CAP_HOST;
        return true;
    }

    error_id = ERR_ARG_API_FIELD;
    return false;
}

bool
as_opt(char *n)
{
    (void) n;

    if (check_field_support(API_CAP_AS)) {
        selected_capabilites |= API_CAP_AS;
        return true;
    }

    error_id = ERR_ARG_API_FIELD;
    return false;
}

bool
asname_opt(char *n)
{
    (void) n;

    if (check_field_support(API_CAP_AS_NAME)) {
        selected_capabilites |= API_CAP_AS_NAME;
        return true;
    }

    error_id = ERR_ARG_API_FIELD;
    return false;
}

bool
isp_opt(char *n) {
    (void) n;

    if (check_field_support(API_CAP_ISP)) {
        selected_capabilites |= API_CAP_ISP;
        return true;
    }

    error_id = ERR_ARG_API_FIELD;
    return false;
}

bool
continent_opt(char *n)
{
    (void) n;

    if (check_field_support(API_CAP_CONTINENT)) {
        selected_capabilites |= API_CAP_CONTINENT;
        return true;
    }

    error_id = ERR_ARG_API_FIELD;
    return false;
}

bool
country_opt(char *n)
{
    (void) n;

    if (check_field_support(API_CAP_COUNTRY)) {
        selected_capabilites |= API_CAP_COUNTRY;
        return true;
    }

    error_id = ERR_ARG_API_FIELD;
    return false;
}

bool
region_opt(char *n) {
    (void) n;

    if (check_field_support(API_CAP_REGION)) {
        selected_capabilites |= API_CAP_REGION;
        return true;
    }

    error_id = ERR_ARG_API_FIELD;
    return false;
}

bool
city_opt(char *n)
{
    (void) n;

    if (check_field_support(API_CAP_CITY)) {
        selected_capabilites |= API_CAP_CITY;
        return true;
    }

    error_id = ERR_ARG_API_FIELD;
    return false;
}

bool
zone_opt(char *n)
{
    (void) n;

    if (check_field_support(API_CAP_TIMEZONE)) {
        selected_capabilites |= API_CAP_TIMEZONE;
        return true;
    }

    error_id = ERR_ARG_API_FIELD;
    return false;
}

bool
is_host_opt(char *n)
{
    (void) n;

    if (check_field_support(API_CAP_ISHOST)) {
        selected_capabilites |= API_CAP_ISHOST;
        return true;
    }

    error_id = ERR_ARG_API_FIELD;
    return false;
}

bool
is_proxy_opt(char *n)
{
    (void) n;

    if (check_field_support(API_CAP_ISPROXY)) {
        selected_capabilites |= API_CAP_ISPROXY;
        return true;
    }

    error_id = ERR_ARG_API_FIELD;
    return false;
}

bool
is_mobile_opt(char *n)
{
    (void) n;

    if (check_field_support(API_CAP_ISMOBILE)) {
        selected_capabilites |= API_CAP_ISMOBILE;
        return true;
    }

    error_id = ERR_ARG_API_FIELD;
    return false;
}

