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

/* Maximum count of
 * supported API parameters.
 */
#define API_CAP_COUNT 14

/* Count of supported APIs. */
#define APIS_COUNT 3

/* Count of supported API
 * parameters by certain API.
 */
#define API_CAPS_COUNT(api_caps) (sizeof (api_caps) / sizeof (struct api_cap_id))

/* Internal IDs for
 * API indication.
 */
enum api_ids {
    IP_API_COM,
    IPAPI_CO,
    EXTREME_IP
};

/* Global enumerator for
 * selected API parameters.
 */
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

/* Represents Certain API parameter,
 * JSON key, pretty printer name,
 * and API response (NULL if not).
 */
struct api_cap_id {
    const enum api_cap capablitiy;
    const char *str_json_key;
    const char *str_value_name;
    char *result;
};

/* Represents one supported API.
 * Contains API ID and it's
 * string representation,
 * pointers to request/response functions,
 * bitset with ALL selected parameters,
 * array with API paramters description.
 */
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

/* Contains a selected
 * API enumerator value.
 */
extern enum api_ids selected_api;

/* Array with description ALL
 * for supported APIs.
 */
extern struct api_node apis_list[];

/* Compares ALL supported string
 * API IDs with passed string
 * and select API when it's found
 * and return `true`, otherwise `false`.
 */
bool
select_api_by_str_id(const char * const str_id);

/* Check if selected API supports
 * passed API parameter.
 */
bool
check_field_support(enum api_cap api_cap);

/* Get point to API structure
 * by enumerator ID.
 */
struct api_node *
get_api_by_id(enum api_ids id);

