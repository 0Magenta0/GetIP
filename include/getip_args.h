/*
 *  include/getip_args.h
 *  This file is a part of the GetIP distribution released under GNU GPLv3
 *  Copyright (C) 2021-2023  0Magenta0
 *  https://github.com/0Magenta0/GetIP
 */

#pragma once

#include <stdbool.h>

extern bool is_verbose;
extern bool is_force;

bool
args_handler(const int argc, char * const argv[]);

