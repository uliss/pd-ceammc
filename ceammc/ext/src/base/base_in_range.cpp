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
#include "base_in_range.h"
#include "ceammc_factory.h"
#include "fmt/format.h"

static t_symbol* SYM_RANGE_CLOSED;
static t_symbol* SYM_RANGE_OPENED;
static t_symbol* SYM_RANGE_OPENED_CLOSED;
static t_symbol* SYM_RANGE_CLOSED_OPEN;

BaseInRange::BaseInRange(const PdArgs& args)
    : BaseObject(args)
    , a_(nullptr)
    , b_(nullptr)
    , cmp_(nullptr)
{
    a_ = new FloatProperty("@l", 0);
    addProperty(a_);

    b_ = new FloatProperty("@u", 0);
    addProperty(b_);

    cmp_ = new SymbolEnumProperty("@cmp", { "[]", "[)", "()", "(]" });
    cmp_->setSuccessFn([this](Property*) {
        const auto v = cmp_->value();
        if (v == SYM_RANGE_CLOSED) {
            type_ = (CMP_GE | CMP_LE);
        } else if (v == SYM_RANGE_OPENED) {
            type_ = (CMP_GT | CMP_LT);
        } else if (v == SYM_RANGE_OPENED_CLOSED) {
            type_ = (CMP_GT | CMP_LE);
        } else if (v == SYM_RANGE_CLOSED_OPEN) {
            type_ = (CMP_GE | CMP_LT);
        } else
            OBJ_ERR << fmt::format("unknown value: {}", v->s_name);
    });
    addProperty(cmp_);
}

void BaseInRange::onFloat(t_float f)
{
    const auto a = a_->value();
    const auto b = b_->value();

    if (type_ & CMP_GE && type_ & CMP_LE) {
        if (a > b) {
            OBJ_ERR << fmt::format("invalid range, expected a<=b, got: {} {}", a, b);
            return;
        }

        floatTo(0, a <= f && f <= b);
    } else if (type_ & CMP_GT && type_ & CMP_LT) {
        if (a >= b) {
            OBJ_ERR << fmt::format("invalid range, expected a<b, got: {} {}", a, b);
            return;
        }
        floatTo(0, a < f && f < b);
    } else if (type_ & CMP_GT && type_ & CMP_LE) {
        if (a >= b) {
            OBJ_ERR << fmt::format("invalid range, expected a<b, got: {} {}", a, b);
            return;
        }
        floatTo(0, a < f && f <= b);
    } else if (type_ & CMP_GE && type_ & CMP_LT) {
        if (a >= b) {
            OBJ_ERR << fmt::format("invalid range, expected a<b, got: {} {}", a, b);
            return;
        }
        floatTo(0, a <= f && f < b);
    } else {
        OBJ_ERR << fmt::format("invalid compare type: {}", type_);
    }
}

void BaseInRange::initDone()
{
    const auto& args = unparsedPosArgs();
    if (args.size() > 2) {
        const auto& a = args[1];
        if (a.isFloat())
            a_->setValue(a.asT<t_float>());
        else if (a.isSymbol()) {
            const auto str = a.asT<t_symbol*>()->s_name;
            const auto len = strlen(str);
            if (len > 0) {

                try {
                    const auto ch = str[len - 1];
                    size_t idx = 0;
                    const auto f = std::stof(str, &idx);

                    if (idx != (len - 1)) {
                        OBJ_ERR << fmt::format("invalid range: {}", str);
                    } else if (ch == ']') {
                        b_->setValue(f);
                        type_ = (type_ & RANGE_B_MASK) | RANGE_CLOSED;
                    } else if (ch == ')') {
                        b_->setValue(f);
                        type_ |= RANGE_CLOSED_OPEN;
                    } else {
                        OBJ_ERR << fmt::format("unknown range type: {}, ] or ) expected", str);
                    }
                } catch (std::exception& e) {
                    OBJ_ERR << fmt::format("can't parse string: {}", str);
                }
            }
        }
    }
}

void setup_base_in_range()
{
    SYM_RANGE_CLOSED = gensym("[]");
    SYM_RANGE_OPENED = gensym("()");
    SYM_RANGE_OPENED_CLOSED = gensym("(]");
    SYM_RANGE_CLOSED_OPEN = gensym("[)");

    // [flow.select 1 -2..3 2|0.1 a|b|c]

    ObjectFactory<BaseInRange> obj("in_range");
    obj.noArgsDataParsing();
}
