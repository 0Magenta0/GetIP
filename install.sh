#!/bin/sh

echo -e "[\033[36m*\033[0m] Checking needed permissions..."
if [ $EUID -ne 0 ]; then
    echo -e "[\033[31m-\033[0m] Run it as root!"
    exit
fi

if [ "$1" = "install" ]; then
    echo -e "[\033[36m*\033[0m] Trying to find /usr/local/bin/ directory..."
    if [ ! -d "/usr/local/bin" ]; then
        echo -e "[\033[31m-\033[0m] /usr/local/bin/ is not found!"
        exit
    fi

    echo -e "[\033[36m*\033[0m] Trying to compile GetIP..."
    gcc *.c -ljson-c -o getip
    if [ $? -ne 0 ]; then
        echo -e "[\033[31m-\033[0m] Compilation is not successful!"
        exit
    fi

    echo -e "[\033[36m*\033[0m] Changing of owner..."
    chown root:root getip
    echo -e "[\033[36m*\033[0m] Changing of permissions..."
    chmod 655 getip
    echo -e "[\033[36m*\033[0m] Moving GetIP to /usr/local/bin/..."
    mv getip "/usr/local/bin"
    echo -e "[\033[32m+\033[0m] Successfuly installed!"
elif [ "$1" = "uninstall" ]; then
    rm -f "/usr/local/bin/getip"
    echo -e "[\033[32m+\033[0m] Successfuly uninstalled!"
    exit
else
    echo -e "[\033[31m-\033[0m] First argument can be only \"install\" or \"uninstall\"!"
    exit
fi

