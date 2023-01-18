/*
 *  src/getip_apis.c
 *  This file is a part of the GetIP distribution released under GNU GPLv3
 *  Copyright (C) 2021-2023  0Magenta0
 *  https://github.com/0Magenta0/GetIP
 */

#include "getip_apis.h"

#include <string.h>
#include <stdbool.h>

#include <json-c/json.h>

enum apis_ids selected_id = IP_API_COM;

struct ip_api supported_apis[APIS_COUNT] = {
    { true,
      { IP_API_COM,
        "IP_API_COM"
      },
      "http://ip-api.com/json/{IP}",
      API_IP | API_COUNTRY | API_HOSTNAME,
      { { API_IP,       "query",    "IP"       },
        { API_COUNTRY,  "country",  "COUNTRY"  },
        { API_HOSTNAME, "hostname", "HOSTNAME" },
      }
    },
};

struct ip_api *
get_api_by_id(enum apis_ids id) {
    for (int counter = 0; counter < APIS_COUNT; ++counter) {
        if (supported_apis[counter].id_name.api_id == id) {
            return &supported_apis[counter];
        }
    }

    return NULL;
}

bool
check_field_support(enum ip_api_fields field) {
    /* Unneccessary NULL check is avoided */
    if (get_api_by_id(selected_id)->bitmap_supported_fields & field) {
        return true;
    }

    return false;
}

bool
select_api_by_str_id(char *str_id) {
    for (int counter = 0; counter < APIS_COUNT; ++counter) {
        if (!strcmp(supported_apis[counter].id_name.api_name, str_id)) {
            selected_id = supported_apis[counter].id_name.api_id;
            return true;
        }
    }

    return false;
}

