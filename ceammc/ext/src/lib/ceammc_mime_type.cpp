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
#include "ceammc_mime_type.h"
#include "fmt/core.h"

#ifdef WITH_LIBMAGIC
#include <magic.h>
#endif

namespace ceammc {

MimeTypeChecker::MimeTypeChecker(const char* libPath)
{
#ifdef WITH_LIBMAGIC
    impl_ = magic_open(MAGIC_SYMLINK
        | MAGIC_CHECK
        | MAGIC_MIME_TYPE
        | MAGIC_ERROR
        | MAGIC_NO_CHECK_TAR
        //        | MAGIC_NO_CHECK_SOFT
        | MAGIC_NO_CHECK_CDF
        | MAGIC_NO_CHECK_CSV
        | MAGIC_NO_CHECK_ELF
        | MAGIC_NO_CHECK_ENCODING
        | MAGIC_NO_CHECK_COMPRESS);

    if (!impl_)
        return;

    size_t bytes_max = 128;
    magic_setparam(static_cast<magic_t>(impl_), MAGIC_PARAM_BYTES_MAX, &bytes_max);

    if (magic_load(static_cast<magic_t>(impl_), libPath) != 0) {
        fmt::print(stderr, "can't load magic database: {}\n", magic_error(static_cast<magic_t>(impl_)));
        magic_close(static_cast<magic_t>(impl_));
        impl_ = nullptr;
        return;
    }

#endif
}

MimeTypeChecker::~MimeTypeChecker()
{
    if (impl_)
        magic_close(static_cast<magic_t>(impl_));
}

std::string MimeTypeChecker::mimeType(const char* file)
{
    std::lock_guard<typeof(mtx_)> guard(mtx_);

    if (!impl_)
        return {};

#ifdef WITH_LIBMAGIC

    auto res = magic_file(static_cast<magic_t>(impl_), file);
    if (!res) {
        fmt::print(stderr, "can't check file '{}': {}\n", file, magic_error(static_cast<magic_t>(impl_)));
        return {};
    } else
        return res;

#else
    return {};
#endif
    return {};
}

}
