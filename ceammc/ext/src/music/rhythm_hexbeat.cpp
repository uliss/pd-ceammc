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
#include "rhythm_hexbeat.h"
#include "ceammc_factory.h"
#include "ceammc_fn_list.h"

#include <cstring>

RhythmHexBeat::RhythmHexBeat(const PdArgs& args)
    : BaseObject(args)
    , hex_(nullptr)
{
    hex_ = new SymbolProperty("@hex", &s_);
    hex_->setArgIndex(0);
    hex_->setSuccessFn([this](Property*) { updatePattern(); });
    addProperty(hex_);

    addProperty(new PointerProperty<AtomList>("@pattern", &pattern_));

    createInlet();
    createOutlet();
}

void RhythmHexBeat::onBang()
{
    listTo(0, pattern_);
}

void RhythmHexBeat::onSymbol(t_symbol* s)
{
    if (hex_->setValue(s))
        updatePattern();

    onBang();
}

void RhythmHexBeat::onList(const AtomListView& lv)
{
    AtomList output;
    output.push_back(Atom(0.));

    int i = 0;
    for (auto& a : lv) {
        output[0] = i++;

        if (a.isSymbol() && hex_->setValue(a.asT<t_symbol*>())) {
            updatePattern();
            output.append(pattern_);
            listTo(0, output);
            output.resizeClip(1);
        }
    }
}

void RhythmHexBeat::onAny(t_symbol* s, const AtomListView& lv)
{
    onSymbol(s);

    for (auto& a : lv) {
        if (a.isSymbol())
            onSymbol(a.asT<t_symbol*>());
    }
}

void RhythmHexBeat::onInlet(size_t n, const AtomListView& lv)
{
    hex_->set(lv);
}

void RhythmHexBeat::updatePattern()
{
    pattern_ = list::hexbeat_bin(hex_->value()->s_name);
}

void setup_rhythm_hexbeat()
{
    ObjectFactory<RhythmHexBeat> obj("rhythm.hexbeat");
    obj.setXletsInfo({ "bang: output last pattern\n"
                       "symbol: set new pattern and output\n"
                       "list: output enumerated for each list argument",
                         "symbol: set new pattern" },
        { "list: 1 and 0 pattern" });

    obj.parseArgsMode(PdArgs::PARSE_UNQUOTE);
    obj.parsePropsMode(PdArgs::PARSE_UNQUOTE);
}
