/*
 *  include/getip_request.h
 *  This file is a part of the GetIP distribution released under GNU GPLv3
 *  Copyright (C) 2021-2023  0Magenta0
 *  https://github.com/0Magenta0/GetIP
 */

#pragma once

#include <stdbool.h>

extern bool is_external_ip;
extern char *external_ip_str;

bool
send_api_request(void);

