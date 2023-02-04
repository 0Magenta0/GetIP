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

/* Structure that contains
 * the targets strings and
 * next target pointer.
 */
extern struct external_ip {
    char   *ip_str;
    size_t str_len;
    struct external_ip *next;
} *external_ip, *last_external_ip;

/* Represents the API response to
 * the parameter and it's key.
 */
struct api_cap_response {
    enum api_cap key;
    char *str_res;
};

/* Indicates if need to
 * request for external
 * targets.
 */
extern bool is_external_ips;

/* Indicates if custom
 * HTTP UserAgent is used.
 */
extern bool is_custom_agent;

/* Indicates if there
 * is more targets.
 */
extern bool is_end;

/* Contains selected API
 * parameters to request.
 */
extern enum api_cap selected_capabilites;

/* Contains string with
 * custom UserAgent.
 */
extern char *custom_agent;

/* Prepare, send request
 * and handle response.
 */
bool
send_api_request(void);

/* Check if HTTP status
 * code is not 200.
 */
bool
curl_check_code(CURL *curl);

