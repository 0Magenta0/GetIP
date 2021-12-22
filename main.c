#include <stdio.h>
#include <stdlib.h>
#include "getip_utils.h"

int main (int argc, char** argv) {
    parameter_handler (argc, argv);
    exec_requests ();
}

