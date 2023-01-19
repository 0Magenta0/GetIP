/*
 *  include/getip_apis.h
 *  This file is a part of the GetIP distribution released under GNU GPLv3
 *  Copyright (C) 2021-2023  0Magenta0
 *  https://github.com/0Magenta0/GetIP
 */

#pragma once

#include <stdbool.h>

#define APIS_COUNT 1
#define FIELDS_COUNT 4

enum apis_ids {
    IP_API_COM
};

enum ip_api_fields {
    API_IP        = 0x01,
    API_COUNTRY   = 0x02,
    API_HOSTNAME  = 0x04,
    API_REGION    = 0x08,
};

struct api_id_name {
    enum apis_ids api_id;
    char *api_name;
};

struct fields_keys {
    enum ip_api_fields current_field;
    char *json_key_str;
    char *getip_field_str;
};

struct ip_api {
    bool is_default;
    struct api_id_name id_name;
    enum ip_api_fields bitmap_supported_fields;
    struct fields_keys supported_fields_keys[FIELDS_COUNT];
};

extern enum apis_ids selected_id;
extern struct ip_api supported_apis[APIS_COUNT];

struct ip_api *
get_api_by_id(enum apis_ids id);

bool
check_field_support(enum ip_api_fields field);

bool
select_api_by_str_id(char *str_id);

bool
ip_api_com_build_request(void);

