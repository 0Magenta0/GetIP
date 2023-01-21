/*
 *  include/getip_errors.h
 *  This file is a part of the GetIP distribution released under GNU GPLv3
 *  Copyright (C) 2021-2023  0Magenta0
 *  https://github.com/0Magenta0/GetIP
 */

#pragma once

enum error_id {
    ERR_IP_STR
};

extern enum error_id error_id;

void
error_handler(void);

