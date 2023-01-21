/*
 *  include/getip_errors.h
 *  This file is a part of the GetIP distribution released under GNU GPLv3
 *  Copyright (C) 2021-2023  0Magenta0
 *  https://github.com/0Magenta0/GetIP
 */

#pragma once

#include <stdbool.h>

enum error_id {
    ERR_IP_STR,
    ERR_ARG_UNK,
    ERR_ARG_MISS
};

extern enum error_id error_id;

void
error_handler(void);

