/*
 *  include/getip_errors.h
 *  This file is a part of the GetIP distribution released under GNU GPLv3
 *  Copyright (C) 2021-2023  0Magenta0
 *  https://github.com/0Magenta0/GetIP
 */

#pragma once

#include <stdbool.h>

/* List of errors that
 * can be handled.
 */
enum error_id {
    ERR_IP_STR = 1,
    ERR_ARG_UNK,
    ERR_ARG_MISS,
    ERR_ARG_CANT_MMDB,
    ERR_ARG_API_UNK,
    ERR_ARG_API_FIELD,
    ERR_ARG_API_NOKEY,
    ERR_ARG_API_REQKEY,
    ERR_ARG_API_KEYLEN,
    ERR_ARG_API_NOTARGET,
    ERR_ARG_API_REQTARGET,
    ERR_ARG_API_WHEN_MMDB,
    ERR_CURL_GLOB_INIT,
    ERR_CURL_EASY_INIT,
    ERR_CURL_EASY_PERF,
    ERR_API_RET_CODE,
    ERR_API_JSON_PARSE,
    ERR_API_STATUS,
    ERR_API_RES_NULL,
    ERR_MMDB_OPEN,
    ERR_MMDB_GAI,
    ERR_MMDB_REQUEST,
    ERR_MMDB_TARGET
};

/* Indicates what
 * is failed.
 */
extern enum error_id error_id;

/* Handle error IDs and
 * print usage if needed.
 */
void
error_handler(void);

