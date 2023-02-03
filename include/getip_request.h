/*
 *  include/getip_request.h
 *  This file is a part of the GetIP distribution released under GNU GPLv3
 *  Copyright (C) 2021-2023  0Magenta0
 *  https://github.com/0Magenta0/GetIP
 */

#pragma once

#include "getip_apis.h"
#include "getip_request.h"

#include <stdlib.h>
#include <stdbool.h>

extern struct external_ip {
    char   *ip_str;
    size_t str_len;
    struct external_ip *next;
} *external_ip, *last_external_ip;

struct api_cap_response {
    enum api_cap key;
    char *str_res;
};

extern bool is_external_ips;
extern bool is_custom_agent;
extern bool is_end;
extern enum api_cap selected_capabilites;
extern char *custom_agent;

bool
send_api_request(void);

bool
curl_check_code(CURL *curl);

