/*
 *  include/getip_errs.h
 *  This file is a part of the GetIP distribution released under GNU GPLv3
 *  Copyright (C) 2021-2023  0Magenta0
 *  https://github.com/0Magenta0/GetIP
 */

#pragma once

enum errs_status {
    ERRS_ARGS_COUNT,
    ERRS_ARGS_IP_STR_LEN,
    ERRS_ARGS_FIELD_SUPPORT,
    ERRS_ARGS_API_NULL,
    ERRS_ARGS_API_UNK,
    ERRS_CURL_GLOB_INIT,
    ERRS_CURL_EASY_INIT,
    ERRS_CURL_EASY_PERF
};

extern enum errs_status errs_status;

void
errs_handler(void);

