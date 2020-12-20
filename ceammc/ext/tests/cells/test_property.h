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
#ifndef TEST_PROPERTY_H
#define TEST_PROPERTY_H

#include "catch.hpp"
#include "ceammc_property.h"

#include "test_base.h"

#include <functional>
#include <initializer_list>

using namespace ceammc;

#define CHECK_READONLY(t, val)                           \
    {                                                    \
        t.setReadOnly();                                 \
        REQUIRE_FALSE(t.set(val));                       \
        REQUIRE_FALSE(t.get() == val);                   \
        REQUIRE_FALSE(t.setInit(val));                   \
        REQUIRE_FALSE(t.get() == val);                   \
        REQUIRE_FALSE(t.setArgIndex(0));                 \
        REQUIRE_FALSE(t.setArgIndexNext(nullptr));       \
        REQUIRE_FALSE(t.setArgIndexNext(&t));            \
        REQUIRE_FALSE(t.setSuccessFn(nullptr));          \
        REQUIRE_FALSE(t.setSuccessFn([](Property*) {})); \
    }

#define CHECK_INITONLY(t, val)         \
    {                                  \
        t.setInitOnly();               \
        REQUIRE_FALSE(t.set(val));     \
        REQUIRE_FALSE(t.get() == val); \
        REQUIRE(t.setInit(val));       \
        REQUIRE(t.get() == val);       \
    }

#define CHECK_SUCCESS(t, ok, not_ok)                      \
    {                                                     \
        int cnt = 0;                                      \
        REQUIRE(t.setSuccessFn([&](Property* prop) { cnt++; REQUIRE((&t) == prop); })); \
        REQUIRE(t.set(ok));                               \
        REQUIRE(cnt == 1);                                \
        REQUIRE(t.setInit(ok));                           \
        REQUIRE(t.get() == ok);                           \
        REQUIRE(cnt == 2);                                \
        REQUIRE_FALSE(t.set(not_ok));                     \
        REQUIRE_FALSE(t.setInit(not_ok));                 \
        REQUIRE(cnt == 2);                                \
        REQUIRE(t.get() == ok);                           \
        REQUIRE(t.set(ok));                               \
        REQUIRE(cnt == 3);                                \
        REQUIRE(t.setInit(ok));                           \
        REQUIRE(cnt == 4);                                \
        t.setSuccessFn(nullptr);                          \
        REQUIRE_FALSE(t.set(not_ok));                     \
        REQUIRE_FALSE(t.setInit(not_ok));                 \
        REQUIRE(t.set(ok));                               \
        REQUIRE(t.setInit(ok));                           \
        REQUIRE(cnt == 4);                                \
    }

#define _ID(id) id
#define _DO_CHECK_SET(t, val)        \
    {                                \
        REQUIRE(t.set(LA(val)));     \
        REQUIRE(t.get() == LA(val)); \
    }

template <typename T>
void REQUIRE_ALL(Property& p, std::function<void(Property&, T)> fn, std::initializer_list<T> l)
{
    for (auto& v : l)
        fn(p, v);
}

#define _SET_FLOAT [](Property& p, t_float new_v) { \
    REQUIRE(p.setFloat(new_v));                     \
    t_float x;                                      \
    REQUIRE(p.getFloat(x));                         \
    REQUIRE(x == new_v);                            \
}

#define _SET_FLOAT_T [](Property& p, t_float new_v) { \
    REQUIRE(p.setT(new_v));                           \
    t_float x;                                        \
    REQUIRE(p.getT(x));                               \
    REQUIRE(x == new_v);                              \
}

#define _CANT_SET_FLOAT [](Property& p, t_float new_v) { \
    t_float v0, v1;                                      \
    REQUIRE(p.getFloat(v0));                             \
    REQUIRE_FALSE(p.setFloat(new_v));                    \
    REQUIRE(p.getFloat(v1));                             \
    REQUIRE(v0 == v1);                                   \
}

#define _CANT_SET_FLOAT_T [](Property& p, t_float new_v) { \
    t_float v0, v1;                                        \
    REQUIRE(p.getT(v0));                                   \
    REQUIRE_FALSE(p.setT(new_v));                          \
    REQUIRE(p.getT(v1));                                   \
    REQUIRE(v0 == v1);                                     \
}

#endif // TEST_PROPERTY_H
