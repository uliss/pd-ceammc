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
#include "ceammc_string.h"

#ifdef __WIN32
#define NS(f) win_##f
#include "ceammc_string_win.h"
#else
#define NS(f) iconv_##f
#include "ceammc_string_iconv.h"
#endif

size_t ceammc::string::utf8_strlen(const char* str)
{
    return NS(utf8_strlen)(str);
}
