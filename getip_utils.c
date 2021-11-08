#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "getip_utils.h"
#include "sock_utils.h"
 
_Bool only4, bExternalIP;

char* externalIP = "";

void printHelp (int exitCode) {
    puts ("Usage: getip <args>\n\n"
          "  -h\t\tPrint this message\n"
          "  -e <str>\tUse another's IP\n"
          "  -4\t\tPrint only IP parameter\n\n"
          "Version: 1.0.1\n"
          "Author: _Magenta_\n");
    exit (exitCode);
}

void parameterHandler (int ac, char** av) {
    opterr = 1;
    for (int arg; (arg = getopt (ac, av, "he:4")) != -1;) {
        switch (arg) {
            case 'h':
                printHelp (0);
                break;

            case 'e':
                bExternalIP = 1;
                externalIP = optarg;
                break;

            case '4':
                only4 = 1;
                break;

            default:
                printHelp (1);
                break;
        }
    }
}

void execRequests () {
    socketInit ();
    releaseRequest ();
}

