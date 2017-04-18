/*****************************************************************************
 * Copyright 2017 Serge Poltavsky. All rights reserved.
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
#include "ceammc_convert.h"

#include <cstdlib>

std::string ceammc::convert::time::sec2str(int sec)
{
    int psec = abs(sec);

    char buf[20];
    char* buf_ptr = buf;
    if (sec < 0) {
        buf_ptr[0] = '-';
        buf_ptr++;
    }

    int h = (psec % (3600 * 24)) / 3600;
    int m = (psec % 3600) / 60;
    int s = psec % 60;
    snprintf(buf_ptr, 18, "%02d:%02d:%02d", h, m, s);
    return buf;
}

