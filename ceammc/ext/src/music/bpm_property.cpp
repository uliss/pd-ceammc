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
#include "bpm_property.h"
#include "ceammc_log.h"
#include "lex/parser_music.h"

#define PROP_ERR() LogPdObject(owner(), LOG_ERROR).stream() << errorPrefix()

namespace ceammc {

BpmProperty::BpmProperty(const std::string& name, t_float init, PropValueAccess access, t_float beatlen)
    : Property(PropertyInfo(name, PropValueType::LIST), access)
    , value_(init)
    , beatlen_(beatlen)
{
    info().setDefault(AtomList(init, beatlen));
}

AtomList BpmProperty::get() const
{
    return { value_, beatlen_ };
}

bool BpmProperty::setList(const AtomListView& lv)
{
    if (!emptyCheck(lv))
        return false;

    if (lv.size() == 2 && lv[0].isFloat() && lv[1].isFloat()) {
        const auto f0 = lv[0].asT<t_float>();
        const auto f1 = lv[1].asT<t_float>();

        if (!setValue(f0))
            return false;

        if (!setBeatlen(f1))
            return false;

        return true;
    } else if (lv.size() == 1 && lv[0].isFloat()) {
        return setFloat(lv[0].asT<t_float>());
    } else if (lv.size() == 1 && lv[0].isSymbol()) {
        using namespace ceammc::parser;
        BpmFullMatch p;
        if (!p.parse(lv[0])) {
            PROP_ERR() << "invalid BPM value: " << lv[0];
            return false;
        }

        value_ = p.bpm().bpm;
        beatlen_ = p.bpm().beatlen;
        return true;
    } else
        return true;
}

bool BpmProperty::setFloat(t_float f)
{
    return setValue(f);
}

bool BpmProperty::getFloat(t_float& f) const
{
    f = value_;
    return true;
}

bool BpmProperty::setValue(t_float f)
{
    if (f <= 0) {
        PROP_ERR() << "positive BPM value expected, got: " << f;
        return false;
    }

    value_ = f;
    return true;
}

AtomList BpmProperty::defaultValue() const
{
    return info().defaultList();
}

bool BpmProperty::setBeatlen(t_float beatlen)
{
    if (beatlen <= 0) {
        PROP_ERR() << "positive duration value expected, got: " << beatlen;
        return false;
    }

    beatlen_ = beatlen;
    return true;
}

t_float BpmProperty::beatPeriodMs() const
{
    return 60000.0 / value_;
}

t_float BpmProperty::wholePeriodMs() const
{
    return beatPeriodMs() / beatlen_;
}

}
