/*
 *  src/getip_apis.c
 *  This file is a part of the GetIP distribution released under GNU GPLv3
 *  Copyright (C) 2021-2023  0Magenta0
 *  https://github.com/0Magenta0/GetIP
 */

#include "getip_apis.h"

#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

enum api_ids selected_api;

const struct api_node apis_list[] = {
    { IP_API_COM,
      "IP_API_COM",
      API_CAP_IP | API_CAP_HOST,
      { { API_CAP_IP,
          "IP"
        },

        { API_CAP_HOST,
          "Hostname"
        }
      }
    }
};

bool
select_api_by_str_id(const char * const str_id)
{
    for (size_t counter = 0; counter < APIS_COUNT; ++counter) {
        if (!strcmp(apis_list[counter].str_id, str_id)) {
            selected_api = apis_list[counter].id;
            return true;
        }
    }

    return false;
}

bool
check_field_support(const enum api_cap api_cap)
{
    return apis_list[selected_api].capabilities & api_cap;
}

