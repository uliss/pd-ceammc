/*****************************************************************************
 * Copyright 2021 Serge Poltavsky. All rights reserved.
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
#include "model.h"
#include "nui/nui.h"

#include "ceammc_log.h"
#include "ceammc_property.h"

#include <cassert>

namespace ceammc {
namespace ui {

    SliderProps::SliderProps(int8_t style)
        : style_idx(style)
    {
        using sc = StyleCollection;
        bd_color = sc::color(style, "slider:border"_hash, bd_color);
        bg_color = sc::color(style, "slider:backgr"_hash, bg_color);
        kn_color = sc::color(style, "slider:knob"_hash, kn_color);
    }

    bool SliderProps::update(const Property* p)
    {
        using namespace ceammc;

        if (p->info().constraints() != PropValueConstraints::CLOSED_RANGE) {
            LIB_ERR << "invalid property constraints: closed_range expected, got: " << (int)p->info().constraints();
            return false;
        }

        switch (p->type()) {
        case PropValueType::FLOAT: {
#ifndef NDEBUG
            auto fp = dynamic_cast<const FloatProperty*>(p);
            assert(fp != nullptr);
#else
            auto fp = static_cast<const FloatProperty*>(p);
#endif
            value = fp->value();
            min = fp->info().minFloat();
            max = fp->info().maxFloat();
            return true;
        } break;
        case PropValueType::INTEGER: {
#ifndef NDEBUG
            auto ip = dynamic_cast<const IntProperty*>(p);
            assert(ip != nullptr);
#else
            auto ip = static_cast<const IntProperty*>(p);
#endif

            value = ip->value();
            min = ip->info().minInt();
            max = ip->info().maxInt();
            return true;
        } break;
        default:
            LIB_ERR << "invalid property type for slider: " << (int)p->type();
            return false;
        }
    }

    LabelProps::LabelProps(int8_t style)
        : style_idx(style)
    {
        using sc = StyleCollection;
        txt_color = sc::color(style, "label:text"_hash, txt_color);
        bd_color = sc::color(style, "label:border"_hash, bd_color);
        bg_color = sc::color(style, "label:backgr"_hash, bg_color);
        font = sc::font(style, "label"_hash, font);
    }

}
}
