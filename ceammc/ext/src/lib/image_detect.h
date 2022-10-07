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
#ifndef IMAGE_DETECT_H
#define IMAGE_DETECT_H

#include <cstddef>
#include <cstdint>

namespace ceammc {
namespace image {

    enum ImageFileType {
        IMAGE_FILE_INVALID = 0,
        IMAGE_FILE_JPG,
        IMAGE_FILE_PNG,
        IMAGE_FILE_GIF,
        IMAGE_FILE_BMP
    };

    ImageFileType detectType(const uint8_t* data, size_t len);
}
}

#endif // IMAGE_DETECT_H
