/*
 *  src/getip_errs.c
 *  This file is a part of the GetIP distribution released under GNU GPLv3
 *  Copyright (C) 2021-2023  0Magenta0
 *  https://github.com/0Magenta0/GetIP
 */

#include "getip_errs.h"

#include <stdio.h>

int errs_status;

void
errs_handler(void) {
    printf("%d\n", errs_status);
}

