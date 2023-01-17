#!/bin/bash
#---------------==================================================---------------#
#  build/builder.sh                                                              #
#  This file is a part of the GetIP distribution released under GNU GPLv3        #
#  Copyright (C) 2021-2023  0Magenta0                                            #
#  https://github.com/0Magenta0/GetIP                                            #
#---------------==================================================---------------#

# Clean (Remove everything except himself)
if [[ $(echo "$1" | grep -Eq "^.*C.*$"; echo $?) -eq 0 ]]; then
    find * ! -iname 'builder.sh' -exec rm -rf {} +
fi

# Build Debug
if [[ $(echo "$1" | grep -Eq "^.*D.*$"; echo $?) -eq 0 ]]; then
    cmake .. -DCMAKE_BULD_TYPE=Debug && make
fi

# Build Release
if [[ $(echo "$1" | grep -Eq "^.*R.*$"; echo $?) -eq 0 ]]; then
    cmake .. && make
fi

