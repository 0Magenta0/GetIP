#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "getip_utils.h"

int main (int argc, char** argv) {
    parameterHandler (argc, argv);
    execRequests ();
}

