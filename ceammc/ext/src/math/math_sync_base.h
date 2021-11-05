/*****************************************************************************
 * Copyright 2019 Serge Poltavsky. All rights reserved.
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
#ifndef MATH_SYNC_BASE_H
#define MATH_SYNC_BASE_H

#include <type_traits>

#include "ceammc_object.h"

namespace ceammc {

typedef std::function<t_float(t_float, t_float)> FloatBinFn;
typedef std::function<bool(bool, bool)> BoolBinFn;
template <bool N>
struct IntT {
};

template <>
struct IntT<false> {
    typedef std::int32_t type;
};

template <>
struct IntT<true> {
    typedef std::int64_t type;
};

typedef IntT<std::is_same<t_float, double>::value>::type IntType;

class MathSyncBase : public BaseObject {
protected:
    t_float v1_, v2_;
    FloatBinFn fn_;
    FlagProperty* prop_int_;

public:
    MathSyncBase(FloatBinFn fn, const PdArgs& args, bool create_int_prop = true);
    void onBang() final;
    void onFloat(t_float f) final;
    void onInlet(size_t n, const AtomListView& lst) final;
    void onList(const AtomList& lst) final;
};

class MathSyncBool : public BaseObject {
protected:
    bool v1_, v2_;
    BoolBinFn fn_;

public:
    MathSyncBool(BoolBinFn fn, const PdArgs& args);
    void onBang() final;
    void onFloat(t_float f) final;
    void onInlet(size_t n, const AtomListView& lst) final;
    void onList(const AtomList& lst) final;

private:
    bool checkBool(t_float f) const;
};

}

#endif // MATH_SYNC_BASE_H
