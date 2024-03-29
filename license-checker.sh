#!/usr/bin/env bash
#---------------==================================================---------------#
#  license-check.sh                                                              #
#  This file is a part of the GetIP distribution released under GNU GPLv3        #
#  Copyright (C) 2021-2023  0Magenta0                                            #
#  https://github.com/0Magenta0/GetIP                                            #
#---------------==================================================---------------#

# File name shouldn't contains the whitespace or wildcards.
for correct_path in $(find * -type f -name '*.c' -or -name '*.h'); do
    if [ "$correct_path" != "$(awk 'BEGIN{} {if(NR == 2){print substr($0,5)}}' $correct_path)" ]; then
        echo -e "[\x1B[31mFAIL - PATH\x1B[0m] $correct_path"
        continue
    fi

    if [ '0Magenta0' != "$(awk 'BEGIN{} {if(NR == 4){print substr($0,30)}}' $correct_path)" ]; then
        echo -e "[\x1B[31mFAIL - NICK\x1B[0m] $correct_path"
        continue
    fi

    if [ 'https://github.com/0Magenta0/GetIP' != "$(awk 'BEGIN{} {if(NR == 5){print substr($0,5)}}' $correct_path)" ]; then
        echo -e "[\x1B[31mFAIL - LINK\x1B[0m] $correct_path"
        continue
    fi
done

