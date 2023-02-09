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

#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include <curl/curl.h>
#include <maxminddb.h>

struct mmdb_field {
    char *value;
};

bool is_mmdb;
bool is_external_ips;
bool is_custom_agent;
bool is_end;
bool is_api_key;
struct external_ip *external_ip;
struct external_ip *last_external_ip;
char *custom_agent;
char *mmdb_file;
struct str_buf api_key;
enum api_cap selected_capabilities;
enum mmdb_cap selected_mmdb_capabilities;
struct MMDB_s mmdb;

/* Append curl HTTP response
 * to buffer structure.
 */
size_t
write_response(void   *npart,
               size_t size,
               size_t nmemb,
               void   *responsep);

/* Send curl HTTP request. */
bool
curl_perform(CURL *curl);

/* Checks if MMDB request
 * is successful.
 */
bool
mmdb_check_error(int mmdb_error,
                 int gai_error);

/* Print pretty output. */
void
print_response(void);

/* Checks if MMDB path
 * request is successful.
 */
bool
mmdb_check_status(int mmdb_error);

/* Sends request to MMDB. */
bool
mmdb_get_data(void);

bool
mmdb_init(void)
{
    int mmdb_status;

    mmdb_status = MMDB_open(mmdb_file, 0, &mmdb);
    if (mmdb_status != MMDB_SUCCESS) {
        if (is_verbose) {
            fprintf(stderr, "getip: can't open %s - %s\n",
                    mmdb_file, MMDB_strerror(mmdb_status));

            if (mmdb_status == MMDB_IO_ERROR) {
                fprintf(stderr, "       IO error: %s\n", strerror(errno));
            }
        }

        error_id = ERR_MMDB_OPEN;
        return false;
    }

    return true;
}

bool
send_mmdb_request(void)
{
    if (!mmdb_get_data()) {
        return false;
    }

    return true;
}

bool
curl_init(void)
{
    if (curl_global_init(CURL_GLOBAL_SSL)) {
        error_id = ERR_CURL_GLOB_INIT;
        return false;
    }

    return true;
}

bool
send_api_request(void)
{
    CURL *curl;
    struct str_buf response = {0};
    char *default_agent;
    struct external_ip *tmp_ip_str;

    if (!(curl = curl_easy_init())) {
        error_id = ERR_CURL_EASY_INIT;

        goto _end_fail;
    }

    if (is_custom_agent) {
        curl_easy_setopt(curl, CURLOPT_USERAGENT, custom_agent);
    } else {
        default_agent = strtok(curl_version() + 3, " ");
        curl_easy_setopt(curl, CURLOPT_USERAGENT, default_agent);
    }

    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_response);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response);

    get_api_by_id(selected_api)->build_request(curl);

    if (!curl_perform(curl)) {
        goto _end_fail;
    }

    curl_easy_cleanup(curl);

    if (is_raw) {
        if (!curl_check_code(curl)) {
            free(response.buf);
            return false;
        }

        fputs(response.buf, stdout);
        free(response.buf);
    } else {
        if (!get_api_by_id(selected_api)->
                handle_response(curl, response.buf, response.len)) {
            if (response.buf) {
                free(response.buf);
            }

            return false;
        }

        free(response.buf);
        print_response();
    }

    if (!is_external_ips) {
        is_end = true;
        return true;
    }

    if (external_ip == last_external_ip) {
        is_end = true;
        free(external_ip->ip_str);
        free(external_ip);
        curl_global_cleanup();
    } else {
        tmp_ip_str = external_ip->next;
        free(external_ip->ip_str);
        free(external_ip);
        external_ip = tmp_ip_str;

        if (!is_raw) {
            if (!is_no_delim) {
                puts("=================================");
            }
        } else {
            putc('\n', stdout);
        }
    }

    return true;

_end_fail:
    curl_global_cleanup();

    return false;
}

bool
curl_check_code(CURL *curl)
{
    long status_code;

    curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &status_code);
    if (status_code != 200) {
        error_id = ERR_API_RET_CODE;
        return false;
    }

    return true;
}

size_t
write_response(void   *npart,
               size_t size,
               size_t nmemb,
               void   *responsep)
{
    const size_t full_size = size * nmemb;
    struct str_buf *tmp_responsep = (struct str_buf *) responsep;
    char *str_responsep;

    if (responsep != NULL) {
        str_responsep = realloc(tmp_responsep->buf,
                tmp_responsep->len + full_size + 1);
    } else {
        str_responsep = malloc(full_size + 1);
    }

    tmp_responsep->buf = str_responsep;
    memcpy(tmp_responsep->buf + tmp_responsep->len, npart, full_size);
    tmp_responsep->len += full_size;
    tmp_responsep->buf[tmp_responsep->len] = '\0';

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

    current_api = get_api_by_id(selected_api);
    caps_count = API_CAPS_COUNT(current_api->api_cap_id);
    for (counter = 0; counter < caps_count; ++counter) {
        if (current_api->api_cap_id[counter].capablitiy & selected_capabilities) {
            if (current_api->api_cap_id[counter].result) {
                printf("[\x1B[0;32m+\x1B[0m] %s: %s\n",
                        current_api->api_cap_id[counter].str_value_name,
                        current_api->api_cap_id[counter].result);
                free(current_api->api_cap_id[counter].result);
            } else {
                printf("[\x1B[0;31m-\x1B[0m] %s: %s\n",
                        current_api->api_cap_id[counter].str_value_name,
                        "\x1B[0;31mFAIL\x1B[0m");
            }
        }
    }
}

bool
mmdb_check_status(int mmdb_error)
{
    if (mmdb_error != MMDB_SUCCESS) {
        if (is_verbose) {
            fprintf(stderr, "getip: libmaxminddb: %s\n", MMDB_strerror(mmdb_error));
        }

        MMDB_close(&mmdb);
        error_id = ERR_MMDB_REQUEST;
        return false;
    }

    return true;
}

bool
mmdb_check_error(int mmdb_error,
                 int gai_error)
{
    if (gai_error != 0) {
        if (is_verbose) {
            fprintf(stderr, "getip: getaddrinfo: %s - %d\n", external_ip->ip_str, gai_error);
        }

        MMDB_close(&mmdb);
        error_id = ERR_MMDB_GAI;
        return false;
    }

    if (mmdb_error != MMDB_SUCCESS) {
        if (is_verbose) {
            fprintf(stderr, "getip: libmaxminddb: %s\n", MMDB_strerror(mmdb_error));
        }

        MMDB_close(&mmdb);
        error_id = ERR_MMDB_REQUEST;
        return false;
    }

    return true;
}

bool
mmdb_get_data(void)
{
    MMDB_entry_data_s mmdb_data;
    MMDB_lookup_result_s mmdb_result;
    struct external_ip *tmp_ip_str;
    int mmdb_error;
    int gai_error;

    mmdb_result = MMDB_lookup_string(&mmdb, external_ip->ip_str, &gai_error, &mmdb_error);

    if (!mmdb_check_error(mmdb_error, gai_error)) {
        return false;
    }

    if (mmdb_result.found_entry) {
        mmdb_error = MMDB_get_value(&mmdb_result.entry, &mmdb_data,
                                    "registered_country", "names", "en", NULL);

        if (mmdb_check_status(mmdb_error)) {
            return false;
        }

        if (mmdb_data.has_data) {
            printf("EN: %.*s\n", mmdb_data.data_size, mmdb_data.utf8_string);
        } else {
            MMDB_close(&mmdb);
            error_id = ERR_MMDB_REQUEST;
            return false;
        }
    } else {
        fputs("getip: couldn't find TARGET entry", stderr);
        MMDB_close(&mmdb);
        error_id = ERR_MMDB_REQUEST;
        return false;
    }

    if (external_ip == last_external_ip) {
        is_end = true;
        free(external_ip->ip_str);
        free(external_ip);
        MMDB_close(&mmdb);
    } else {
        tmp_ip_str = external_ip->next;
        free(external_ip->ip_str);
        free(external_ip);
        external_ip = tmp_ip_str;

        if (!is_raw) {
            if (!is_no_delim) {
                puts("=================================");
            }
        } else {
            putc('\n', stdout);
        }
    }

    return true;
}

