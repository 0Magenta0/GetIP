/*
 *  include/getip_errs.h
 *  This file is a part of the GetIP distribution released under GNU GPLv3
 *  Copyright (C) 2021-2023  0Magenta0
 *  https://github.com/0Magenta0/GetIP
 */

#pragma once

enum errs_status {
    ERRS_ARGS_COUNT
};

extern int errs_status;

void errs_handler(void);

