/*
 *  src/getip_usage.c
 *  This file is a part of the GetIP distribution released under GNU GPLv3
 *  Copyright (C) 2021-2023  0Magenta0
 *  https://github.com/0Magenta0/GetIP
 */

#include "getip_usage.h"

#include <stdio.h>

void
print_usage(void) {
    puts("GetIP v" GETIP_VERSION "\n"
         GETIP_DESCRIPTION "\n\n"
         "Usage: getip [target] [options]"
    );
}

