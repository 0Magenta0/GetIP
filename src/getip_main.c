/*
 *  src/getip_main.c
 *  This file is a part of the GetIP distribution released under GNU GPLv3
 *  Copyright (C) 2021-2023  0Magenta0
 *  https://github.com/0Magenta0/GetIP
 */

#include "getip_args.h"
#include "getip_errors.h"
#include "getip_request.h"

#include <stdio.h>
#include <stdlib.h>

int
main(int  argc,
     char *argv[])
{
    if (args_handler(argc, argv)) {
        /* While have new IPs
         * send new requests.
         */
        while (!is_end) {
            /* If request is failure
             * exit with error.
             */
            if (!send_api_request()) {
                error_handler();
                exit(EXIT_FAILURE);
            }
        }
    } else {
        /* Handle errors
         * when have bad args.
         */
        error_handler();
        exit(EXIT_FAILURE);
    }

    exit(EXIT_SUCCESS);
}

