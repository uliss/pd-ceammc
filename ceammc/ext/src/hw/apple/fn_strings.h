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
#ifndef FN_STRINGS_H
#define FN_STRINGS_H

#include <MacTypes.h>
#include <cstddef>

typedef char SMCBytes[32];
typedef char UInt32CharKey[5];

UInt32 smc_bytes_to_int(const SMCBytes& str, int size, int base);
float smc_bytes_to_float(const SMCBytes& str, int size, int e);
float smc_bytes_sp78_to_float(const SMCBytes& str);
void smc_int_to_key(const UInt32 int_key, UInt32CharKey char_key);

#endif // FN_STRINGS_H
