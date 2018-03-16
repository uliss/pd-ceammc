/*****************************************************************************
 * Copyright 2018 Serge Poltavsky. All rights reserved.
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
#include "image_detect.h"

#include <cstring>

namespace ceammc {
namespace image {

    ImageFileType detectType(const uint8_t* data, size_t len)
    {
        if (len < 16)
            return IMAGE_FILE_INVALID;

        switch (data[0]) {
        case (uint8_t)'\xFF':
            return (!strncmp((const char*)data, "\xFF\xD8\xFF", 3))
                ? IMAGE_FILE_JPG
                : IMAGE_FILE_INVALID;

        case (uint8_t)'\x89':
            return (!strncmp((const char*)data,
                       "\x89\x50\x4E\x47\x0D\x0A\x1A\x0A", 8))
                ? IMAGE_FILE_PNG
                : IMAGE_FILE_INVALID;

        case 'G':
            return (!strncmp((const char*)data, "GIF87a", 6) || !strncmp((const char*)data, "GIF89a", 6))
                ? IMAGE_FILE_GIF
                : IMAGE_FILE_INVALID;

        case 'B':
            return ((data[1] == 'M'))
                ? IMAGE_FILE_BMP
                : IMAGE_FILE_INVALID;

        default:
            return IMAGE_FILE_INVALID;
        }
    }
}
}
