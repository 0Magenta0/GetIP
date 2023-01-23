/*
 *  src/getip_apis.c
 *  This file is a part of the GetIP distribution released under GNU GPLv3
 *  Copyright (C) 2021-2023  0Magenta0
 *  https://github.com/0Magenta0/GetIP
 */

#include "getip_apis.h"
#include "getip_args.h"
#include "getip_errors.h"
#include "getip_request.h"

#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include <curl/curl.h>
#include <json-c/json.h>

#define IP_API_COM_URL     "http://ip-api.com/json/"
#define IP_API_COM_URL_LEN (sizeof (IP_API_COM_URL) / sizeof (char))

enum ip_api_com_bitset {
    BS_IP_API_COM_IP        =    0x2000,
    BS_IP_API_COM_ORG       =    0x0400,
    BS_IP_API_COM_HOST      =    0x1000,
    BS_IP_API_COM_AS        =    0x0800,
    BS_IP_API_COM_AS_NAME   =  0x400000,
    BS_IP_API_COM_ISP       =    0x0200,
    BS_IP_API_COM_CONTINENT =  0x100000,
    BS_IP_API_COM_COUNTRY   =      0x01,
    BS_IP_API_COM_REGION    =      0x08,
    BS_IP_API_COM_CITY      =      0x10,
    BS_IP_API_COM_TIMEZONE  =     0x100,
    BS_IP_API_COM_ISHOST    = 0x1000000,
    BS_IP_API_COM_ISPROXY   =   0x20000,
    BS_IP_API_COM_ISMOBILE  =   0x10000
};

void
ip_api_com_builder(CURL *curl);

bool
ip_api_com_handler(CURL *curl,
                   char *json_response,
                   size_t json_res_len);

enum api_ids selected_api;

struct api_node apis_list[] = {
    { IP_API_COM,
      "IP_API_COM",
      ip_api_com_builder,
      ip_api_com_handler,
      0x3FFF,
      { { API_CAP_IP,
          "query",
          "IP",
          NULL
        },

        { API_CAP_ORG,
          "org",
          "ORG",
          NULL
        },

        { API_CAP_HOST,
          "hostname",
          "Hostname",
          NULL
        },

        { API_CAP_AS,
          "as",
          "AS",
          NULL
        },

        { API_CAP_AS_NAME,
          "asname",
          "AS Name",
          NULL
        },

        { API_CAP_ISP,
          "isp",
          "ISP",
          NULL
        },

        { API_CAP_CONTINENT,
          "continent",
          "Continent",
          NULL
        },

        { API_CAP_COUNTRY,
          "country",
          "Country",
          NULL
        },

        { API_CAP_REGION,
          "regionName",
          "Region",
          NULL
        },

        { API_CAP_CITY,
          "city",
          "City",
          NULL
        },

        { API_CAP_TIMEZONE,
          "timezone",
          "Time Zone",
          NULL
        },

        { API_CAP_ISHOST,
          "hosting",
          "Hosting",
          NULL
        },

        { API_CAP_ISPROXY,
          "proxy",
          "Proxy",
          NULL
        },

        { API_CAP_ISMOBILE,
          "mobile",
          "Mobile",
          NULL
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

struct api_node *
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
    int tmp_bitset = 0;

    if (selected_capabilites & API_CAP_IP) {
        tmp_bitset |= BS_IP_API_COM_IP;
    } if (selected_capabilites & API_CAP_ORG) {
        tmp_bitset |= BS_IP_API_COM_ORG;
    } if (selected_capabilites & API_CAP_HOST) {
        tmp_bitset |= BS_IP_API_COM_HOST;
    } if (selected_capabilites & API_CAP_AS) {
        tmp_bitset |= BS_IP_API_COM_AS;
    } if (selected_capabilites & API_CAP_AS_NAME) {
        tmp_bitset |= BS_IP_API_COM_AS_NAME;
    } if (selected_capabilites & API_CAP_ISP) {
        tmp_bitset |= BS_IP_API_COM_ISP;
    } if (selected_capabilites & API_CAP_CONTINENT) {
        tmp_bitset |= BS_IP_API_COM_CONTINENT;
    } if (selected_capabilites & API_CAP_COUNTRY) {
        tmp_bitset |= BS_IP_API_COM_COUNTRY;
    } if (selected_capabilites & API_CAP_REGION) {
        tmp_bitset |= BS_IP_API_COM_REGION;
    } if (selected_capabilites & API_CAP_CITY) {
        tmp_bitset |= BS_IP_API_COM_CITY;
    } if (selected_capabilites & API_CAP_TIMEZONE) {
        tmp_bitset |= BS_IP_API_COM_TIMEZONE;
    } if (selected_capabilites & API_CAP_ISHOST) {
        tmp_bitset |= BS_IP_API_COM_ISHOST;
    } if (selected_capabilites & API_CAP_ISPROXY) {
        tmp_bitset |= BS_IP_API_COM_ISPROXY;
    } if (selected_capabilites & API_CAP_ISMOBILE) {
        tmp_bitset |= BS_IP_API_COM_ISMOBILE;
    }

    tmp_bitset |= 0xC000 /* Two flag enabled by default */;

    tmp_url = malloc(external_ip.str_len + /* By default is 0 */
                     IP_API_COM_URL_LEN + /* Includes Null-terminator */
                     16 /* API bitset */);
    if (is_external_ip) {
        sprintf(tmp_url, IP_API_COM_URL "%.*s?fields=%d", MAX_IP_STR_LEN, external_ip.ip_str, tmp_bitset);
        curl_easy_setopt(curl, CURLOPT_URL, tmp_url);
    } else {
        sprintf(tmp_url, IP_API_COM_URL "?fields=%d", tmp_bitset);
        curl_easy_setopt(curl, CURLOPT_URL, tmp_url);
    }

    free(tmp_url);
}

bool
ip_api_com_handler(CURL   *curl,
                   char   *json_response,
                   size_t json_res_len)
{
    struct api_node *current_api;
    size_t caps_count;
    size_t cap_res_len;

    struct json_object  *parsed_json;
    struct json_object  *certain_json_obj;
    struct json_tokener *tokener_ex;

    long status_code;
    size_t counter;

    curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &status_code);
    if (status_code != 200) {
        error_id = ERR_API_RET_CODE;
        return false;
    }

    tokener_ex = json_tokener_new();
    if (!(parsed_json = json_tokener_parse_ex(tokener_ex, json_response, (int) json_res_len))) {
        if (is_verbose) {
            fprintf(stderr, "JSON error: %d\n JSON buffer:\n%s\n", json_tokener_get_error(tokener_ex), json_response);
        }

        error_id = ERR_API_JSON_PARSE;
        json_tokener_free(tokener_ex);
        return false;
    } json_tokener_free(tokener_ex);

    json_object_object_get_ex(parsed_json, "status", &certain_json_obj);
    if (strcmp(json_object_get_string (certain_json_obj), "success")) {
        if (is_verbose) {
            printf("API error: Response status is \"%s\"\n", json_object_get_string(certain_json_obj));
            json_object_object_get_ex(parsed_json, "message", &certain_json_obj);
            printf("API message: %s\n", json_object_get_string(certain_json_obj));
        }

        error_id = ERR_API_STATUS;
        return false;
    }

    current_api = get_api_by_id(IP_API_COM);
    caps_count = API_CAPS_COUNT(current_api->api_cap_id);
    for (counter = 0; counter < caps_count; ++counter) {
        if (current_api->api_cap_id[counter].capablitiy & selected_capabilites) {
            json_object_object_get_ex(parsed_json, current_api->api_cap_id[counter].str_json_key, &certain_json_obj);
            if (!json_object_is_type(certain_json_obj, json_type_null)
                && (json_object_get_string_len(certain_json_obj)
                   || json_object_is_type(certain_json_obj, json_type_boolean))) {
                if (json_object_is_type(certain_json_obj, json_type_boolean)) {
                    current_api->api_cap_id[counter].result = malloc(6);
                    strncpy(current_api->api_cap_id[counter].result, json_object_get_string(certain_json_obj), 6);
                } else {
                    cap_res_len = (size_t) json_object_get_string_len(certain_json_obj);
                    current_api->api_cap_id[counter].result = malloc(cap_res_len + 1);
                    strncpy(current_api->api_cap_id[counter].result, json_object_get_string(certain_json_obj), cap_res_len);
                }
            }
        }
    }

    return true;
}

