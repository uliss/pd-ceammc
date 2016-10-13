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
#include "ceammc_string_iconv.h"

#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <iostream>

#if defined(__FreeBSD__)
#define LIBICONV_PLUG
#endif

#include <iconv.h>

static const size_t BUF_SIZE = 100;
static const size_t MAX_UNICODE_CHAR_WIDTH = 4;

class IconvHandle {
public:
    iconv_t cd;

public:
    IconvHandle()
    {
        cd = iconv_open("UTF-32LE", "UTF-8");
        if (cd == reinterpret_cast<iconv_t>(-1)) {
            std::cerr << "can't convert from UTF-8 to UTF-16LE" << std::endl;
        }
    }

    ~IconvHandle()
    {
        iconv_close(cd);
    }
};

static wchar_t* utf8towchar(const char* utf8)
{
    char* input = strdup(utf8);
    char* def_copy = input;
    size_t ibl = strlen(input); // Input length
    size_t obl = MAX_UNICODE_CHAR_WIDTH * ibl; // Max output length

    wchar_t* text = reinterpret_cast<wchar_t*>(malloc(obl * sizeof(wchar_t)));
    char* output = reinterpret_cast<char*>(text);

    IconvHandle ih;
    iconv(ih.cd, &input, &ibl, &output, &obl);
    free(def_copy);

    return text;
}

size_t iconv_utf8_strlen(const char* str)
{
    char abuf[BUF_SIZE] = { 0 };
    char wbuf[MAX_UNICODE_CHAR_WIDTH * BUF_SIZE];
    char* text = 0;

    size_t len = strlen(str);
    char *inbuf, *outbuf;
    size_t inbytesleft, outbytesleft, outbuf_size;

    // short strings
    if (len < BUF_SIZE - 1) {
        strncpy(abuf, str, BUF_SIZE - 1);
        abuf[BUF_SIZE - 1] = '\0';

        inbuf = abuf;
        inbytesleft = len;
        outbuf = reinterpret_cast<char*>(wbuf);
        outbuf_size = sizeof(wbuf);
        outbytesleft = outbuf_size;
    } else {
        inbuf = strdup(str);
        inbytesleft = len;
        outbuf_size = MAX_UNICODE_CHAR_WIDTH * len;
        outbytesleft = outbuf_size;
        text = reinterpret_cast<char*>(malloc(outbuf_size));
        outbuf = reinterpret_cast<char*>(text);
    }

    IconvHandle ih;
    size_t res = iconv(ih.cd, &inbuf, &inbytesleft, &outbuf, &outbytesleft);

    if (text)
        free(text);

    return (res == static_cast<size_t>(-1)) ? 0 : (outbuf_size - outbytesleft) / MAX_UNICODE_CHAR_WIDTH;
}
