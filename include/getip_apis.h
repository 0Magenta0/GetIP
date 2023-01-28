/*
 *  include/getip_apis.h
 *  This file is a part of the GetIP distribution released under GNU GPLv3
 *  Copyright (C) 2021-2023  0Magenta0
 *  https://github.com/0Magenta0/GetIP
 */

#pragma once

#include <stdlib.h>
#include <stdbool.h>

#include <curl/curl.h>

#define API_CAP_COUNT 14
#define APIS_COUNT 2

#define API_CAPS_COUNT(api_caps) (sizeof (api_caps) / sizeof (struct api_cap_id))

enum api_ids {
    IP_API_COM,
    IPAPI_CO
};

enum api_cap {
    API_CAP_IP        =   0x01,
    API_CAP_ORG       =   0x02,
    API_CAP_HOST      =   0x04,
    API_CAP_AS        =   0x08,
    API_CAP_AS_NAME   =   0x10,
    API_CAP_ISP       =   0x20,
    API_CAP_CONTINENT =   0x40,
    API_CAP_COUNTRY   =   0x80,
    API_CAP_REGION    = 0x0100,
    API_CAP_CITY      = 0x0200,
    API_CAP_TIMEZONE  = 0x0400,
    API_CAP_ISHOST    = 0x0800,
    API_CAP_ISPROXY   = 0x1000,
    API_CAP_ISMOBILE  = 0x2000
};

struct api_cap_id {
    const enum api_cap capablitiy;
    const char *str_json_key;
    const char *str_value_name;
    char *result;
};

struct api_node {
    const enum api_ids id;
    const char *str_id;
    void (* const build_request)(CURL *);
    bool (* const handle_response)(CURL *,
                                   char *,
                                   size_t);
    const enum api_cap capabilities;
    struct api_cap_id api_cap_id[API_CAP_COUNT];
};

extern enum api_ids selected_api;

extern struct api_node apis_list[];

bool
select_api_by_str_id(const char * const str_id);

bool
check_field_support(enum api_cap api_cap);

struct api_node *
get_api_by_id(enum api_ids id);

