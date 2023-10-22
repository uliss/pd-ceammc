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
#ifndef CEAMMC_SYMBOLS_H
#define CEAMMC_SYMBOLS_H

#include "m_pd.h"

#define CEAMMC_DEFINE_SYMBOL2(name, value)    \
    constexpr const char* str_##name = value; \
    static inline t_symbol* sym_##name() { return gensym(value); }

#define CEAMMC_DEFINE_SYMBOL(name) \
    CEAMMC_DEFINE_SYMBOL2(name, #name)

#define CEAMMC_DEFINE_METHOD2(name, value) \
    namespace methods {                    \
    CEAMMC_DEFINE_SYMBOL2(name, value);    \
    }

#define CEAMMC_DEFINE_METHOD(name) \
    CEAMMC_DEFINE_METHOD2(name, #name)

#define CEAMMC_DEFINE_PROPERTY2(name, value)                                 \
    namespace props {                                                        \
    constexpr const char* value_##name = "@" value;                          \
    constexpr const char* name_##name = value;                               \
    static inline t_symbol* sym_name_##name() { return gensym(value); }      \
    static inline t_symbol* sym_value_##name() { return gensym("@" value); } \
    }

#define CEAMMC_DEFINE_PROPERTY(name) \
    CEAMMC_DEFINE_PROPERTY2(name, #name)

namespace ceammc {
namespace sym {

    // common
    CEAMMC_DEFINE_METHOD(dsp)
    CEAMMC_DEFINE_METHOD(dsp_add)
    CEAMMC_DEFINE_METHOD(dump)
    CEAMMC_DEFINE_METHOD(pos)
    CEAMMC_DEFINE_METHOD(reset)
    CEAMMC_DEFINE_METHOD(vis)
    CEAMMC_DEFINE_METHOD(zoom)

    CEAMMC_DEFINE_SYMBOL(anything)
    CEAMMC_DEFINE_SYMBOL(bang)
    CEAMMC_DEFINE_SYMBOL(float)
    CEAMMC_DEFINE_SYMBOL(list)
    CEAMMC_DEFINE_SYMBOL(symbol)

    // properties
    CEAMMC_DEFINE_PROPERTY2(all, "*")
    CEAMMC_DEFINE_PROPERTY2(all_get, "*?")
}
}

#endif // CEAMMC_SYMBOLS_H
