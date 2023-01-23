/*
 *  src/getip_request.c
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

struct response {
    char *response;
    size_t len;
};

bool is_external_ip;
bool is_custom_agent;
struct external_ip external_ip;
char *custom_agent;
enum api_cap selected_capabilites;

size_t
write_response(void   *npart,
               size_t size,
               size_t nmemb,
               void   *responsep);

bool
curl_perform(CURL *curl);

void
print_response(void);

bool
send_api_request(void)
{
    CURL *curl;
    struct response response = {0};

    if (curl_global_init(CURL_GLOBAL_NOTHING)) {
        error_id = ERR_CURL_GLOB_INIT;

        return false;
    }

    if (!(curl = curl_easy_init())) {
        error_id = ERR_CURL_EASY_INIT;

        goto _end_fail;
    }

    if (is_custom_agent) {
        curl_easy_setopt(curl, CURLOPT_USERAGENT, custom_agent);
    }

    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_response);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response);

    get_api_by_id(selected_api)->build_request(curl);

    if (!curl_perform(curl)) {
        goto _end_fail;
    }

    curl_easy_cleanup(curl);
    curl_global_cleanup();

    if (!get_api_by_id(selected_api)->
            handle_response(curl, response.response, response.len)) {
        return false;
    }

    print_response();

    return true;

_end_fail:
    curl_global_cleanup();

    return false;
}

size_t
write_response(void   *npart,
               size_t size,
               size_t nmemb,
               void   *responsep)
{
    const size_t full_size = size * nmemb;
    struct response *tmp_responsep = (struct response *) responsep;
    char *str_responsep;

    if (responsep != NULL) {
        str_responsep = realloc(tmp_responsep->response,
                tmp_responsep->len + full_size + 1);
    } else {
        str_responsep = malloc(full_size + 1);
    }

    tmp_responsep->response = str_responsep;
    memcpy(tmp_responsep->response + tmp_responsep->len, npart, full_size);
    tmp_responsep->len += full_size;
    tmp_responsep->response[tmp_responsep->len] = '\0';

    return full_size;
}

bool
curl_perform(CURL *curl)
{
    CURLcode curl_status;

    if ((curl_status = curl_easy_perform(curl)) != CURLE_OK) {
        error_id = ERR_CURL_EASY_PERF;
        if (is_verbose) {
            fprintf (stderr, "libcurl: %s\n", curl_easy_strerror(curl_status));
        }

        curl_easy_cleanup(curl);

        return false;
    }

    return true;
}

void
print_response(void)
{
    struct api_node *current_api;
    size_t caps_count;
    size_t counter;

    current_api = get_api_by_id(IP_API_COM);
    caps_count = API_CAPS_COUNT(current_api->api_cap_id);
    for (counter = 0; counter < caps_count; ++counter) {
        if (current_api->api_cap_id[counter].capablitiy & selected_capabilites) {
            if (current_api->api_cap_id[counter].result) {
                printf("[\x1B[0;32m+\x1B[0m] %s: %s\n",
                        current_api->api_cap_id[counter].str_value_name,
                        current_api->api_cap_id[counter].result);
            } else {
                printf("[\x1B[0;31m-\x1B[0m] %s: %s\n",
                        current_api->api_cap_id[counter].str_value_name,
                        "\x1B[0;31mFAIL\x1B[0m");
            }
        }
    }
}

