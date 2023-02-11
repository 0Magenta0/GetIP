/*
 *  include/getip_args.h
 *  This file is a part of the GetIP distribution released under GNU GPLv3
 *  Copyright (C) 2021-2023  0Magenta0
 *  https://github.com/0Magenta0/GetIP
 */

#pragma once

#include <stdbool.h>

#define MIN_IP_STR_LEN   3
#define MAX_IP_STR_LEN 254

extern bool is_verbose;  /* Verbose is needed.          */
extern bool is_raw;      /* Print raw JSON.             */
extern bool is_no_delim; /* Don't draw delimiters
                            between few output targets. */
extern bool is_force;    /* Ignore some warnings. */

/* Handle `argv` parameters.
 * Return `false` when failed
 * and `true` if do not.
 */
bool
args_handler(int argc,
             char * const argv[]);

