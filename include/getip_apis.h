/*
 *  include/getip_apis.h
 *  This file is a part of the GetIP distribution released under GNU GPLv3
 *  Copyright (C) 2021-2023  0Magenta0
 *  https://github.com/0Magenta0/GetIP
 */

#pragma once

#include <stdbool.h>

#define API_CAP_COUNT 2
#define APIS_COUNT 1

enum api_ids {
    IP_API_COM
};

enum api_cap {
    API_CAP_IP      = 0x01,
    API_CAP_HOST    = 0x02,
    API_CAP_COUNTRY = 0x04
};

struct api_cap_id {
    const enum api_cap capablitiy;
    const char *str_value_name;
};

struct api_node {
    const enum api_ids id;
    const char *str_id;
    const enum api_cap capabilities;
    const struct api_cap_id api_cap_id[API_CAP_COUNT];
};

extern enum api_ids selected_api;

extern const struct api_node apis_list[];

bool
select_api_by_str_id(const char * const str_id);

bool
check_field_support(enum api_cap);

