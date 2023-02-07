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

/* Contains API URLs, it's lenght
 * and bitset with ALL selected
 * API parameters.
 */
#define IP_API_COM_URL      "http://ip-api.com/json/"
#define IP_API_COM_URL_LEN  (sizeof (IP_API_COM_URL) / sizeof (char))
#define IP_API_COM_ALL_CAPS 0x3FFF

#define IPAPI_CO_URL      "https://ipapi.co/"
#define IPAPI_CO_URL_LEN  (sizeof (IPAPI_CO_URL) / sizeof (char))
#define IPAPI_CO_ALL_CAPS 0x07CF

#define EXTREME_IP_URL      "https://extreme-ip-lookup.com/json/"
#define EXTREME_IP_URL_LEN  (sizeof (EXTREME_IP_URL) / sizeof (char))
#define EXTREME_IP_ALL_CAPS 0x07FB

#define DB_IP_URL      "https://api.db-ip.com/v2/"
#define DB_IP_URL_LEN  (sizeof (DB_IP_URL) / sizeof (char))
#define DB_IP_ALL_CAPS 0x17F3

/* Bitset that unique
 * to ip-api.com.
 */
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

/* Returns `true` is parsed
 * succefuly and `false` if do not.
 */
bool
json_parse(struct json_object **parsed_json,
           char   *json_response,
           size_t json_res_len);

/* Handle selected API's parameters. */
void
json_copy_caps_values(struct json_object **parsed_json);

/* Get deep json object by key. */
struct json_object *
get_json_obj_by_key(struct json_object **parsed_json,
                    const char *json_key);

/* Check if some API returns
 * a successful message.
 */
bool
check_api_success(struct json_object **parsed_json);

/* Prepare request for ip-api.com. */
void
ip_api_com_builder(CURL *curl);

/* Handle response from ip-api.com. */
bool
ip_api_com_handler(CURL *curl,
                   char *json_response,
                   size_t json_res_len);

/* Prepare request for ipapi.co. */
void
ipapi_co_builder(CURL *curl);

/* Handle response from ipapi.co. */
bool
ipapi_co_handler(CURL   *curl,
                 char   *json_response,
                 size_t json_res_len);

/* Prepare request for extreme-ip-lookup.com. */
void
extreme_ip_builder(CURL *curl);

/* Handle response from extreme-ip-lookup.com. */
bool
extreme_ip_handler(CURL   *curl,
                   char   *json_response,
                   size_t json_res_len);

/* Prepare request for api.db-ip.com. */
void
db_ip_builder(CURL *curl);

/* Handle response from api.db-ip.com. */
bool
db_ip_handler(CURL   *curl,
                   char   *json_response,
                   size_t json_res_len);

enum api_ids selected_api;

struct api_node apis_list[APIS_COUNT] = {
    { IP_API_COM,
      "IP_API_COM",
      false, /* Cannot be used with API key. */
      false, /* API key is not reuqired.     */
      true,  /* Can use TARGET.              */
      false, /* TARGET is not required.      */
      ip_api_com_builder,
      ip_api_com_handler,
      IP_API_COM_ALL_CAPS,
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
          "reverse",
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
    },

    { IPAPI_CO,
      "IPAPI_CO",
      true,  /* Can be used with API key. */
      false, /* API key is not required.  */
      true,  /* Can use TARGET.           */
      false, /* TARGET is not required.   */
      ipapi_co_builder,
      ipapi_co_handler,
      IPAPI_CO_ALL_CAPS,
      { { API_CAP_IP,
          "ip",
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
          "asn",
          "ASN",
          NULL
        },

        { API_CAP_CONTINENT,
          "continent_code",
          "Continent",
          NULL
        },

        { API_CAP_COUNTRY,
          "country_name",
          "Country",
          NULL
        },

        { API_CAP_REGION,
          "region",
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
        }
      }
    },

    { EXTREME_IP,
      "EXTREME_IP",
      true,  /* Can be used with API key. */
      false, /* API key is not required.  */
      true,  /* Can use TARGET.           */
      false, /* TARGET is not required.   */
      extreme_ip_builder,
      extreme_ip_handler,
      IPAPI_CO_ALL_CAPS,
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

        { API_CAP_AS,
          "asn",
          "AS",
          NULL
        },

        { API_CAP_AS_NAME,
          "asName",
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
          "region",
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
        }
      }
    },

    { DB_IP,
      "DB_IP",
      true,  /* Can be used with API key. */
      false, /* API key is not required.  */
      true,
      false,
      db_ip_builder,
      db_ip_handler,
      DB_IP_ALL_CAPS,
      { { API_CAP_IP,
          "ipAddress",
          "IP",
          NULL
        },

        { API_CAP_ORG,
          "organization",
          "ORG",
          NULL
        },

        { API_CAP_AS_NAME,
          "asName",
          "AS NAME",
          NULL
        },

        { API_CAP_ISP,
          "isp",
          "ISP",
          NULL
        },

        { API_CAP_CONTINENT,
          "continentName",
          "Continent",
          NULL
        },

        { API_CAP_COUNTRY,
          "countryName",
          "Country",
          NULL
        },

        { API_CAP_REGION,
          "stateProv",
          "State",
          NULL
        },

        { API_CAP_CITY,
          "city",
          "City",
          NULL
        },

        { API_CAP_TIMEZONE,
          "timeZone",
          "Time Zone",
          NULL
        },

        { API_CAP_ISPROXY,
          "isProxy",
          "Proxy",
          NULL
        },
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

bool
json_parse(struct json_object **parsed_json,
           char   *json_response,
           size_t json_res_len)
{
    struct json_tokener *json_tokener;

    json_tokener = json_tokener_new();

    if (!(*parsed_json = json_tokener_parse_ex(json_tokener, json_response, (int) json_res_len))) {
        if (is_verbose) {
            fprintf(stderr, "JSON error: %d\n JSON buffer:\n%s\n", json_tokener_get_error(json_tokener), json_response);
        }

        error_id = ERR_API_JSON_PARSE;
        json_tokener_free(json_tokener);

        return false;
    }

    json_tokener_free(json_tokener);

    return true;
}

void
json_copy_caps_values(struct json_object **parsed_json)
{
    struct json_object *certain_json_obj;
    struct api_node *current_api;
    size_t caps_count;
    size_t cap_res_len;
    size_t counter;

    current_api = get_api_by_id(selected_api);
    caps_count = API_CAPS_COUNT(current_api->api_cap_id);
    for (counter = 0; counter < caps_count; ++counter) {
        if (current_api->api_cap_id[counter].capablitiy & selected_capabilites) {
            certain_json_obj = get_json_obj_by_key(parsed_json, current_api->api_cap_id[counter].str_json_key);
            if (!json_object_is_type(certain_json_obj, json_type_null)
                && (json_object_get_string_len(certain_json_obj)
                || json_object_is_type(certain_json_obj, json_type_boolean))) {
                if (json_object_is_type(certain_json_obj, json_type_boolean)) {
                    current_api->api_cap_id[counter].result = malloc(6);
                    strncpy(current_api->api_cap_id[counter].result, json_object_get_string(certain_json_obj), 6);
                } else {
                    cap_res_len = (size_t) json_object_get_string_len(certain_json_obj);
                    current_api->api_cap_id[counter].result = malloc(cap_res_len + 1);
                    strcpy(current_api->api_cap_id[counter].result, json_object_get_string(certain_json_obj));
                }
            }
        }
    }

    json_object_put(*parsed_json);
}

struct json_object *
get_json_obj_by_key(struct json_object **parsed_json,
                    const char *json_key)
{
    struct json_object *certain_json_obj;
    char   *tmp_str;
    char   *tmp_str_pos;
    size_t counter = 0;
    size_t str_len;

    str_len = strlen(json_key);
    tmp_str = malloc(str_len + 1);
    tmp_str_pos = tmp_str;
    strcpy(tmp_str, json_key);

    tmp_str = strtok(tmp_str, ".");
    while (tmp_str != NULL) {
        if (!counter) {
            certain_json_obj = json_object_object_get(*parsed_json, tmp_str);
        } else {
            certain_json_obj = json_object_object_get(certain_json_obj, tmp_str);
        }

        tmp_str = strtok(NULL, ".");
        ++counter;
    }

    if (!counter) {
        certain_json_obj = json_object_object_get(*parsed_json, json_key);
        goto _end;
    }

_end:
    free(tmp_str_pos);
    return certain_json_obj;
}

bool
check_api_success(struct json_object **parsed_json)
{
    struct json_object *certain_json_obj;
    const char *tmp_str;

    certain_json_obj = get_json_obj_by_key(parsed_json, "status");
    tmp_str = json_object_get_string(certain_json_obj);
    if (!tmp_str || strcmp(tmp_str, "success")) {
        if (is_verbose) {
            if (!tmp_str) {
                puts("API error: Response status is null");
            } else {
                printf("API error: Response status is \"%s\"\n", tmp_str);
            }

            certain_json_obj = get_json_obj_by_key(parsed_json, "message");
            tmp_str = json_object_get_string(certain_json_obj);
            if (!tmp_str) {
                puts("API message is null");
            } else {
                printf("API message: %s\n", tmp_str);
            }
        }

        json_object_put(*parsed_json);
        error_id = ERR_API_STATUS;
        return false;
    }

    return true;
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

    if (is_external_ips) {
        tmp_url = malloc(external_ip->str_len
                         + IP_API_COM_URL_LEN /* Includes Null-terminator */
                         + 16 /* API bitset */);
    } else {
        tmp_url = malloc(IP_API_COM_URL_LEN /* Includes Null-terminator */
                         + 16 /* API bitset */);
    }

    if (is_external_ips) {
        sprintf(tmp_url, IP_API_COM_URL "%.*s?fields=%d", MAX_IP_STR_LEN, external_ip->ip_str, tmp_bitset);
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
    struct json_object *parsed_json = NULL;

    if (!curl_check_code(curl)) {
        return false;
    }

    if (!json_parse(&parsed_json, json_response, json_res_len)) {
        return false;
    }

    if (!check_api_success(&parsed_json)) {
        return false;
    }

    json_copy_caps_values(&parsed_json);

    return true;
}

void
ipapi_co_builder(CURL *curl)
{
    char *tmp_url;

    curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, true);

    if (is_external_ips) {
        if (is_api_key) {
            tmp_url = malloc(external_ip->str_len
                             + api_key.len
                             + IPAPI_CO_URL_LEN /* Includes Null-terminator */
                             + 10 /* /json?key= */);
        } else {
            tmp_url = malloc(external_ip->str_len
                             + IPAPI_CO_URL_LEN /* Includes Null-terminator */
                             + 5 /* /json */);
        }
    } else {
        if (is_api_key) {
            tmp_url = malloc(IPAPI_CO_URL_LEN /* Includes Null-terminator */
                             + api_key.len
                             + 9 /* json?key= */);
        } else {
            tmp_url = malloc(IPAPI_CO_URL_LEN /* Includes Null-terminator */
                             + 4 /* json */);
        }
    }

    if (is_external_ips) {
        if (is_api_key) {
            sprintf(tmp_url, IPAPI_CO_URL "%.*s/json?key=%.*s", MAX_IP_STR_LEN,
                    external_ip->ip_str, (int) api_key.len, api_key.buf);
            curl_easy_setopt(curl, CURLOPT_URL, tmp_url);
        } else {
            sprintf(tmp_url, IPAPI_CO_URL "%.*s/json", MAX_IP_STR_LEN, external_ip->ip_str);
            curl_easy_setopt(curl, CURLOPT_URL, tmp_url);
        }
    } else {
        if (is_api_key) {
            sprintf(tmp_url, IPAPI_CO_URL "json?key=%.*s", (int) api_key.len, api_key.buf);
            curl_easy_setopt(curl, CURLOPT_URL, tmp_url);
        } else {
            strcpy(tmp_url, IPAPI_CO_URL "json");
            curl_easy_setopt(curl, CURLOPT_URL, tmp_url);
        }
    }

    free(tmp_url);
}

bool
ipapi_co_handler(CURL   *curl,
                 char   *json_response,
                 size_t json_res_len)
{
    struct json_object *parsed_json = NULL;

    if (!curl_check_code(curl)) {
        return false;
    }

    if (!json_parse(&parsed_json, json_response, json_res_len)) {
        return false;
    }

    json_copy_caps_values(&parsed_json);

    return true;
}

void
extreme_ip_builder(CURL *curl)
{
    char *tmp_url;

    if (is_external_ips) {
        if (is_api_key) {
            tmp_url = malloc(external_ip->str_len
                             + EXTREME_IP_URL_LEN /* Includes Null-terminator */
                             + 5 /* ?key= */
                             + api_key.len);
        } else {
            tmp_url = malloc(external_ip->str_len
                             + EXTREME_IP_URL_LEN /* Includes Null-terminator */
                             + 10 /* ?key=demo2 */);
        }
    } else {
        if (is_api_key) {
            tmp_url = malloc(EXTREME_IP_URL_LEN /* Includes Null-terminator */
                             + 5 /* ?key= */
                             + api_key.len);
        } else {
            tmp_url = malloc(EXTREME_IP_URL_LEN /* Includes Null-terminator */
                             + 10 /* ?key=demo2 */);
        }
    }

    if (is_external_ips) {
        if (is_api_key) {
            sprintf(tmp_url, EXTREME_IP_URL "%.*s?key=%.*s", MAX_IP_STR_LEN, external_ip->ip_str, (int) api_key.len, api_key.buf);
            curl_easy_setopt(curl, CURLOPT_URL, tmp_url);
        } else {
            sprintf(tmp_url, EXTREME_IP_URL "%.*s?key=demo2", MAX_IP_STR_LEN, external_ip->ip_str);
            curl_easy_setopt(curl, CURLOPT_URL, tmp_url);
        }
    } else {
        if (is_api_key) {
            sprintf(tmp_url, EXTREME_IP_URL "?key=%.*s", (int) api_key.len, api_key.buf);
            curl_easy_setopt(curl, CURLOPT_URL, tmp_url);
        } else {
            sprintf(tmp_url, EXTREME_IP_URL "?key=demo2");
            curl_easy_setopt(curl, CURLOPT_URL, tmp_url);
        }
    }

    free(tmp_url);
}

bool
extreme_ip_handler(CURL   *curl,
                   char   *json_response,
                   size_t json_res_len)
{
    struct json_object *parsed_json = NULL;

    if (!curl_check_code(curl)) {
        return false;
    }

    if (!json_parse(&parsed_json, json_response, json_res_len)) {
        return false;
    }

    if (!check_api_success(&parsed_json)) {
        return false;
    }

    json_copy_caps_values(&parsed_json);

    return true;
}

void
db_ip_builder(CURL *curl)
{
    char *tmp_url;

    if (is_external_ips) {
        if (is_api_key) {
            tmp_url = malloc(external_ip->str_len
                             + DB_IP_URL_LEN /* Includes Null-terminator */
                             + 1 /* / */
                             + api_key.len);
        } else {
            tmp_url = malloc(external_ip->str_len
                             + DB_IP_URL_LEN /* Includes Null-terminator */
                             + 5 /* free/ */);
        }
    } else {
        if (is_api_key) {
            tmp_url = malloc(DB_IP_URL_LEN /* Includes Null-terminator */
                             + 5 /* /self */
                             + api_key.len);
        } else {
            tmp_url = malloc(DB_IP_URL_LEN /* Includes Null-terminator */
                             + 9 /* free/self */);
        }
    }

    if (is_external_ips) {
        if (is_api_key) {
            sprintf(tmp_url, DB_IP_URL "%.*s/%.*s", (int) api_key.len, api_key.buf, MAX_IP_STR_LEN, external_ip->ip_str);
            curl_easy_setopt(curl, CURLOPT_URL, tmp_url);
        } else {
            sprintf(tmp_url, DB_IP_URL "free/%.*s", MAX_IP_STR_LEN, external_ip->ip_str);
            curl_easy_setopt(curl, CURLOPT_URL, tmp_url);
        }
    } else {
        if (is_api_key) {
            sprintf(tmp_url, DB_IP_URL "%.*s/self", (int) api_key.len, api_key.buf);
            curl_easy_setopt(curl, CURLOPT_URL, tmp_url);
        } else {
            sprintf(tmp_url, DB_IP_URL "free/self");
            curl_easy_setopt(curl, CURLOPT_URL, tmp_url);
        }
    }

    free(tmp_url);
}

bool
db_ip_handler(CURL   *curl,
                   char   *json_response,
                   size_t json_res_len)
{
    struct json_object *parsed_json = NULL;

    if (!curl_check_code(curl)) {
        return false;
    }

    if (!json_parse(&parsed_json, json_response, json_res_len)) {
        return false;
    }

    json_copy_caps_values(&parsed_json);

    return true;
}

