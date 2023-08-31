/*****************************************************************************
 * Copyright 2019 Serge Poltavsky. All rights reserved.
 *
 * This file may be distributed under the terms of GNU Public License version
 * 3 (GPL v3) as defined by the Free Software Foundation (FSF). A copy of the
 * license should have been included with this file, or the project in which
 * this file belongs to. You may also find the details of GPL v3 at:
 * http://www.gnu.org/licenses/gpl-3.0.txt
 *
 * If you have any questions regarding the use of this file, feel free to
 * contact the author of this file, or the owner of the project in which
 * this file belongs to.
 *****************************************************************************/
#include <cstdio>
#include <string.h>
#include <sys/types.h>

#include "fn_strings.h"

void smc_int_to_key(const UInt32 int_key, UInt32CharKey char_key)
{
    char_key[0] = '\0';
    sprintf(char_key, "%c%c%c%c",
        (unsigned int)int_key >> 24,
        (unsigned int)int_key >> 16,
        (unsigned int)int_key >> 8,
        (unsigned int)int_key);
}

UInt32 smc_bytes_to_int(const SMCBytes& str, int size, int base)
{
    UInt32 total = 0;

    for (int i = 0; i < size; i++) {
        if (base == 16)
            total += str[i] << (size - 1 - i) * 8;
        else
            total += (unsigned char)(str[i] << (size - 1 - i) * 8);
    }
    return total;
}

float smc_bytes_to_float(const SMCBytes& str, int size, int e)
{
    float total = 0;

    for (int i = 0; i < size; i++) {
        if (i == (size - 1))
            total += (str[i] & 0xff) >> e;
        else
            total += str[i] << (size - 1 - i) * (8 - e);
    }

    return total;
}

float smc_bytes_sp78_to_float(const SMCBytes& str)
{
    int intValue = int(str[0]) * 256 + int(str[1]);
    return intValue / 256.f;
}
