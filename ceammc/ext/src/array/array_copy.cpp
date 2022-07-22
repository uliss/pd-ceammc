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
#include "array_copy.h"
#include "ceammc_array.h"
#include "ceammc_factory.h"

#include <algorithm>
#include <cassert>
#include <limits>

ArrayCopy::ArrayCopy(const PdArgs& a)
    : BaseObject(a)
    , resize_(nullptr)
{
    createOutlet();

    resize_ = new BoolProperty("@resize", false);
    addProperty(resize_);
}

void ArrayCopy::onList(const AtomListView& lv)
{
    if (!checkArgs(lv, ARG_SYMBOL, ARG_SYMBOL, gensym("copy")))
        return;

    copy(lv[0].asSymbol(), lv[1].asSymbol());
}

void ArrayCopy::m_copy(t_symbol*, const AtomListView& lv)
{
    /// copy array1 array2
    if (lv.size() == 2 && lv.allOf(isSymbol))
        return copy(lv[0].asSymbol(), lv[1].asSymbol());

    /// copy array1 0 100 array2
    if (lv.size() == 4
        && lv[0].isSymbol()
        && lv[1].isFloat()
        && lv[2].isFloat()
        && lv[3].isSymbol()) {
        Range in_range(lv[1].asInt(), lv[2].asInt());
        return copyRange(lv[0].asSymbol(), in_range, lv[3].asSymbol(), 0);
    }

    /// copy array1 0 array2
    if (lv.size() == 3
        && lv[0].isSymbol()
        && lv[1].isFloat()
        && lv[2].isSymbol()) {
        Range in_range(lv[1].asInt(), std::numeric_limits<size_t>::max());
        return copyRange(lv[0].asSymbol(), in_range, lv[2].asSymbol(), 0);
    }

    if (lv.size() == 5
        && lv[0].isSymbol()
        && lv[1].isFloat()
        && lv[2].isFloat()
        && lv[3].isSymbol()
        && lv[4].isFloat()) {
        Range in_range(lv[1].asInt(), lv[2].asInt());
        return copyRange(lv[0].asSymbol(), in_range, lv[3].asSymbol(), lv[4].asInt());
    }

    OBJ_ERR << "invalid arguments are given: " << lv;
}

void ArrayCopy::copy(t_symbol* src, t_symbol* dest)
{
    Array asrc(src);
    Array adest(dest);

    if (!checkArrays(asrc, adest))
        return;

    size_t n = 0;

    if (!resize_->value())
        n = std::min(asrc.size(), adest.size());
    else {
        if (asrc.size() > adest.size()) {
            if (!adest.resize(asrc.size())) {
                OBJ_ERR << "can't resize " << adest.name() << " to " << asrc.size();
                return;
            }
        }

        n = asrc.size();
    }

    std::copy(asrc.begin(), asrc.begin() + ssize_t(n), adest.begin());
    adest.redraw();

    bangTo(0);
}

void ArrayCopy::copyRange(t_symbol* src, const Range& range, t_symbol* dest, size_t destpos)
{
    Array in(src);
    Array out(dest);

    if (!checkArrays(in, out))
        return;

    // invalid from position
    if (range.from >= in.size() || !range.isValid()) {
        OBJ_ERR << "invalid range: " << range.from << '-' << range.to;
        return;
    }

    // clip to source size
    if (!resize_->value()) {
        if (destpos >= out.size()) {
            OBJ_ERR << "invalid destination position: " << destpos;
            return;
        }

        Range in_range = range.clip(in.size());
        Range out_range = Range(destpos, destpos + in_range.length()).clip(out.size());
        ArrayIterator in_from = in.begin() + in_range.from;
        ArrayIterator in_to = in_from + out_range.length();
        ArrayIterator out_from = out.begin() + destpos;
        std::copy(in_from, in_to, out_from);
    } else {
        if (destpos > out.size()) {
            OBJ_ERR << "invalid destination position: " << destpos;
            return;
        }

        Range in_range = range.clip(in.size());
        Range out_range = Range(destpos, destpos + in_range.length());
        if (out_range.to >= out.size())
            out.resize(out_range.to);

        ArrayIterator in_from = in.begin() + in_range.from;
        ArrayIterator in_to = in_from + out_range.length();
        ArrayIterator out_from = out.begin() + destpos;

        std::copy(in_from, in_to, out_from);
    }

    out.redraw();
    bangTo(0);
}

Range Range::clip(size_t max) const
{
    return { from, std::min(to, max) };
}

bool ArrayCopy::checkArrays(const Array& src, const Array& dest) const
{
    if (!src.isValid()) {
        OBJ_ERR << "invalid source array: " << src.name();
        return false;
    }

    if (!dest.isValid()) {
        OBJ_ERR << "invalid destination array: " << dest.name();
        return false;
    }

    return true;
}

void setup_array_copy()
{
    ObjectFactory<ArrayCopy> obj("array.copy");
    obj.addMethod("copy", &ArrayCopy::m_copy);
}
