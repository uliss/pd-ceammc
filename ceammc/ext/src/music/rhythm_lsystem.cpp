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
#include "rhythm_lsystem.h"
#include "ceammc_factory.h"
#include "ceammc_fn_list.h"
#include "datatype_dict.h"

constexpr int MAX_VALUE = 32 * 1024;

RhythmLsystem::RhythmLsystem(const PdArgs& args)
    : BaseObject(args)
    , init_(nullptr)
    , rule_(nullptr)
    , value_(nullptr)
    , max_(nullptr)
{
    createOutlet();

    init_ = new ListProperty("@init");
    init_->setArgIndex(0);
    addProperty(init_);

    rule_ = new DictProperty("@rule", DataTypeDict());
    addProperty(rule_);

    value_ = new ListProperty("@value");
    value_->setReadOnly();
    addProperty(value_);

    max_ = new IntProperty("@max", 256);
    max_->checkClosedRange(1, MAX_VALUE);
    addProperty(max_);
}

void RhythmLsystem::initDone()
{
    BaseObject::initDone();
    value_->setValue(init_->value());
}

void RhythmLsystem::onBang()
{
    auto val = list::lsystem(value_->value(), rule_->value());
    if (val.size() > max_->value())
        val.resizeClip(max_->value());

    value_->setValue(val);
    listTo(0, val);
}

void RhythmLsystem::m_reset(t_symbol* s, const AtomListView& lv)
{
    value_->setValue(init_->value());
}

void setup_rhythm_lsystem()
{
    ObjectFactory<RhythmLsystem> obj("rhythm.lsystem");
    obj.addMethod("reset", &RhythmLsystem::m_reset);
}
