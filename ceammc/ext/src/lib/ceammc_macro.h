/*****************************************************************************
 * Copyright 2020 Serge Poltavsky. All rights reserved.
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
#ifndef CEAMMC_MACRO_H
#define CEAMMC_MACRO_H

#include "m_pd.h"

#if defined(__has_feature)
#if __has_feature(address_sanitizer)
#define CEAMMC_NO_ASAN __attribute__((no_sanitize("address"))) __attribute__((no_sanitize("undefined")))
#else
#define CEAMMC_NO_ASAN
#endif
#else
#define CEAMMC_NO_ASAN
#endif

#if defined(__GNUC__) || defined(__clang__)
#define CEAMMC_DEPRECATED __attribute__((deprecated))
#elif defined(_MSC_VER)
#define CEAMMC_DEPRECATED __declspec(deprecated)
#else
#pragma message("WARNING: You need to implement CEAMMC_DEPRECATED for this compiler")
#define CEAMMC_DEPRECATED
#endif

#if defined(__GNUC__) || defined(__clang__)
#define CEAMMC_WARN_UNUSED __attribute__((warn_unused_result))
#define CEAMMC_PACKED __attribute__((packed))
#else
#define CEAMMC_WARN_UNUSED
#define CEAMMC_PACKED
#endif

#endif // CEAMMC_MACRO_H
