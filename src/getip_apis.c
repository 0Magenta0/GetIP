/*
 *  src/getip_apis.c
 *  This file is a part of the GetIP distribution released under GNU GPLv3
 *  Copyright (C) 2021-2023  0Magenta0
 *  https://github.com/0Magenta0/GetIP
 */

#include "getip_apis.h"
#include "getip_request.h"

#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include <curl/curl.h>

#define IP_API_COM_URL     "http://ip-api.com/json/"
#define IP_API_COM_URL_LEN (sizeof (IP_API_COM_URL) / sizeof (char))

void
ip_api_com_builder(CURL *curl);

enum api_ids selected_api;

const struct api_node apis_list[] = {
    { IP_API_COM,
      "IP_API_COM",
      ip_api_com_builder,
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

const struct api_node *
get_api_by_id(enum api_ids id)
{
    for (size_t counter = 0; counter < APIS_COUNT; ++counter) {
        if (apis_list[counter].id == id) {
            return &apis_list[counter];
        }
    }

    return NULL;
}

void
ip_api_com_builder(CURL *curl)
{
    char *tmp_url;

    if (is_external_ip) {
        tmp_url = malloc(external_ip.str_len + IP_API_COM_URL_LEN);
        sprintf(tmp_url, IP_API_COM_URL "%s", external_ip.ip_str);
        curl_easy_setopt(curl, CURLOPT_URL, tmp_url);
        free(tmp_url);
    } else {
        curl_easy_setopt(curl, CURLOPT_URL, "http://ip-api.com/json");
    }
}

