/*****************************************************************************
 *
 * Created: 04.12.2016 2016 by Serge Poltavski
 *
 * Copyright 2016 Serge Poltavski. All rights reserved.
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

#include "ceammc.hpp"
#include <algorithm>
#include <cassert>
#include <cstring>
#include <limits>

namespace ceammc {
namespace pd {

    bool atoms_minmax(const atom_list& lst, t_float* min, t_float* max)
    {
        assert(min);
        assert(max);

        if (lst.empty())
            return false;

        t_float a_min = std::numeric_limits<t_float>::max();
        t_float a_max = -std::numeric_limits<t_float>::max();
        bool found = false;

        for (size_t i = 0; i < lst.size(); i++) {
            t_float v;
            if (get_float(lst[i], &v)) {
                a_min = std::min(a_min, v);
                a_max = std::max(a_max, v);
                found = true;
            }
        }

        if (!found)
            return false;

        *min = a_min;
        *max = a_max;
        return true;
    }

    bool atoms_normalize(atom_list& lst)
    {
        if (lst.empty())
            return false;

        // find min/max
        t_float a_min = 0;
        t_float a_max = 0;
        if (!atoms_minmax(lst, &a_min, &a_max))
            return false;

        // if equal values - set all to 1
        if (a_max == a_min) {
            for (size_t i = 0; i < lst.size(); i++) {
                if (is_float(lst[i])) {
                    SETFLOAT(&lst[i], 1);
                }
            }
        } else {
            for (size_t i = 0; i < lst.size(); i++) {
                t_float v;
                if (get_float(lst[i], &v)) {
                    t_float n = (v - a_min) / (a_max - a_min);
                    SETFLOAT(&lst[i], n);
                }
            }
        }
        return true;
    }

    AtomList::AtomList(int argc, t_atom* argv)
    {
        if (argc < 0) {
            error("[ceammc] ivalid atom list size: %i. Should be >= 0.", argc);
            return;
        }

        lst_.assign(argv, argv + argc);
    }

    AtomList::AtomList(const atom_list& lst)
        : lst_(lst)
    {
    }

    void AtomList::append(t_float v)
    {
        t_atom a;
        SETFLOAT(&a, v);
        lst_.push_back(a);
    }

    ControlValue::ControlValue()
        : type_(NONE)
        , float_v_(0)
        , symbol_v_(0)
    {
    }

    ControlValue::ControlValue(t_float v)
        : type_(FLOAT)
        , float_v_(v)
        , symbol_v_(0)
    {
    }

    ControlValue::ControlValue(const t_symbol* s)
        : type_(SYMBOL)
        , float_v_(0)
        , symbol_v_(s)
    {
    }

    ControlValue::ControlValue(const atom_list& lst)
        : type_(LIST)
        , float_v_(0)
        , symbol_v_(0)
        , list_v_(lst)
    {
    }

    ControlValue::ControlValue(int argc, t_atom* argv)
        : type_(LIST)
        , float_v_(0)
        , symbol_v_(0)
        , list_v_(argv, argv + argc)
    {
    }

    ControlValue::ControlValue(t_symbol* s, int argc, t_atom* argv)
        : type_(ANY)
        , float_v_(0)
        , symbol_v_(s)
        , list_v_(argv, argv + argc)
    {
    }

    ControlValue::ControlValue(const ControlValue& v)
        : type_(v.type_)
        , float_v_(v.float_v_)
        , symbol_v_(v.symbol_v_)
        , list_v_(v.list_v_)
    {
    }

    ControlValue& ControlValue::operator=(const ControlValue& v)
    {
        type_ = v.type_;
        float_v_ = v.float_v_;
        symbol_v_ = v.symbol_v_;
        list_v_ = v.list_v_;
        return *this;
    }

    bool ControlValue::isEqual(const ControlValue& v) const
    {
        return (*this) == v;
    }

    void ControlValue::output(t_outlet* o)
    {
        switch (type_) {
        case FLOAT:
            outlet_float(o, float_v_);
            break;
        case SYMBOL:
            outlet_symbol(o, const_cast<t_symbol*>(symbol_v_));
            break;
        case LIST:
            ::ceammc::pd::output(o, list_v_);
            break;
        case ANY:
            outlet_anything(o,
                const_cast<t_symbol*>(symbol_v_),
                static_cast<int>(list_v_.size()),
                list_v_.data());
            break;
        case NONE:
            break;
        }
    }

    bool operator==(const ControlValue& c1, const ControlValue& c2)
    {
        if (c1.type_ != c2.type_)
            return false;

        switch (c1.type_) {
        case ControlValue::NONE:
            return true;
        case ControlValue::FLOAT:
            return ceammc_floats_equal(c1.float_v_, c2.float_v_) == 1;
        case ControlValue::SYMBOL:
            if (c1.symbol_v_ == c2.symbol_v_)
                return true;

            if (c1.symbol_v_ == 0 || c2.symbol_v_ == 0)
                return false;

            return strcmp(c1.symbol_v_->s_name, c2.symbol_v_->s_name) == 0;
        case ControlValue::LIST:
            return c1.list_v_ == c2.list_v_;
        case ControlValue::ANY:
            return strcmp(c1.symbol_v_->s_name, c2.symbol_v_->s_name) == 0 && c1.list_v_ == c2.list_v_;
        }
    }

    bool operator!=(const ControlValue& c1, const ControlValue& c2)
    {
        return !(c1 == c2);
    }

    bool operator==(const atom_list& l1, const atom_list& l2)
    {
        if (l1.size() != l2.size())
            return false;

        return std::equal(l1.begin(), l1.end(), l2.begin(), atom_equals);
    }

    bool operator!=(const atom_list& l1, const atom_list& l2)
    {
        return !(l1 == l2);
    }
}
}
