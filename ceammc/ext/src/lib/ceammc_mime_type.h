/*****************************************************************************
 * Copyright 2023 Serge Poltavski. All rights reserved.
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
#ifndef CEAMMC_MIME_TYPE_H
#define CEAMMC_MIME_TYPE_H

#include "ceammc_containers.h"

#include <mutex>
#include <string>

namespace ceammc {

/**
 * thread safe MimeTypeChecker
 */
class MimeTypeChecker {
    void* impl_ { nullptr };
    std::mutex mtx_;

public:
    MimeTypeChecker(const char* libPath = nullptr);
    ~MimeTypeChecker();
    std::string mimeType(const char* file);
};

using MimeTypeLibrary = SingletonMeyers<MimeTypeChecker>;

}

#endif // CEAMMC_MIME_TYPE_H
