/*
 *  src/main.c
 *  This file is a part of the GetIP distribution released under GNU GPLv3
 *  Copyright (C) 2021-2023  0Magenta0
 *  https://github.com/0Magenta0/GetIP
 */

#include "getip_args.h"
#include "getip_errs.h"
#include "getip_request.h"

#include <stdio.h>
#include <stdlib.h>

int
main(int argc, char *argv[]) {
    if (args_handler(argc, argv)) {
        if (send_request()) {
            exit(EXIT_SUCCESS);
        } else {
            errs_handler();
        }
    } else {
        errs_handler();
        exit(EXIT_FAILURE);
    }
}

