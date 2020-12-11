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
#include "array_grainer.h"
#include "ceammc_convert.h"
#include "ceammc_factory.h"
#include "grainprops.lexer.h"
#include "grainprops.parser.hpp"

#include <limits>
#include <random>

ArrayGrainer::ArrayGrainer(const PdArgs& args)
    : SoundExternal(args)
    , array_name_(nullptr)
    , speed_(nullptr)
{
    createSignalOutlet();
    createSignalOutlet();

    array_name_ = new SymbolProperty("@array", &s_);
    array_name_->setArgIndex(0);
    addProperty(array_name_);

    speed_ = new FloatProperty("@speed", 1);
    speed_->checkMin(0.001);
    speed_->setSuccessFn([this](Property*) { cloud_.setSpeed(speed_->value()); });
    addProperty(speed_);
}

void ArrayGrainer::setupDSP(t_signal** sp)
{
    if (!array_.open(array_name_->value())) {
        OBJ_ERR << "can't open array: " << array_name_->value();
        dsp_add_zero(sp[0]->s_vec, sp[0]->s_n);
        return;
    }

    cloud_.setArrayData(array_.begin(), array_.size());

    SoundExternal::setupDSP(sp);
}

void ArrayGrainer::processBlock(const t_sample** in, t_sample** out)
{
    cloud_.playBuffer(out, blockSize());
}

void ArrayGrainer::dump() const
{
    OBJ_POST << "cloud:\n"
                "    num grains: "
             << cloud_.size() << "\n"
                                 "    speed:      "
             << cloud_.speed() << "\n"
                                  "    length:    "
             << cloud_.length();

    OBJ_POST << "grains: ";
    for (auto g : cloud_.grains())
        OBJ_POST << *g.grain;

    SoundExternal::dump();
}

void ArrayGrainer::m_grain(t_symbol* s, const AtomListView& lv)
{
    if (!array_.open(array_name_->value())) {
        METHOD_ERR(s) << "can't open array: " << array_name_->value();
        return;
    }

    auto grain = GrainPool::instance().allocate(0, 1);
    if (!grain) {
        METHOD_ERR(s) << "too many grains, can't insert";
        return;
    }

    GrainPropertiesLexer lex(lv, grain);
    lex.setArraySize(array_.size());

    GrainPropertiesParser parser(lex);

    if (parser.parse() != 0) {
        LIB_ERR << "parse error";
        GrainPool::instance().free(grain);
        return;
    }

    cloud_.addGrain(grain);
}

void ArrayGrainer::m_fill(t_symbol* s, const AtomListView& lv)
{
    if (!array_.open(array_name_->value())) {
        METHOD_ERR(s) << "can't open array: " << array_name_->value();
        return;
    }

    const auto n_grains = lv.intAt(0, 10);
    GrainPropertiesLexer lex(lv.subView(1), nullptr);
    lex.setArraySize(array_.size());

    for (int i = 0; i < n_grains; i++) {
        auto grain = GrainPool::instance().allocate(0, 1);
        if (!grain) {
            METHOD_ERR(s) << "too many grains, can't insert";
            break;
        }

        lex.setGrain(grain);
        lex.reset();
        GrainPropertiesParser parser(lex);

        if (parser.parse() != 0) {
            LIB_ERR << "parse error: " << lv.subView(1);
            GrainPool::instance().free(grain);
            return;
        }

        LIB_POST << "added: " << *grain;
        cloud_.addGrain(grain);
    }
}

void ArrayGrainer::m_clear(t_symbol* s, const AtomListView& lv)
{
    cloud_.clear();
}

void setup_array_grainer()
{
    SoundExternalFactory<ArrayGrainer> obj("array.grainer~", OBJECT_FACTORY_DEFAULT);
    obj.addMethod("grain", &ArrayGrainer::m_grain);
    obj.addMethod("fill", &ArrayGrainer::m_fill);
    obj.addMethod("clear", &ArrayGrainer::m_clear);
}
