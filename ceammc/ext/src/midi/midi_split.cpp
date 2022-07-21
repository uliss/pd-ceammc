/*****************************************************************************
 * Copyright 2022 Serge Poltavsky. All rights reserved.
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
#include "midi_split.h"
#include "ceammc_args.h"
#include "ceammc_factory.h"
#include "lex/parser_music.h"

#include <memory>

static std::unique_ptr<ArgChecker> onlist_chk;

MidiSplit::MidiSplit(const PdArgs& args)
    : BaseObject(args)
{
    ceammc::parser::SpnFullMatch spn_parser;

    for (auto& a : parsedPosArgs()) {
        t_float pitch = 0;

        if (a.isFloat())
            pitch = a.asT<t_float>();
        else if (a.isSymbol() && spn_parser.parse(a))
            pitch = spn_parser.spn().midi();
        else {
            OBJ_ERR << "unknown atom: " << a << "; MIDI pitch or SPN notation expected, skipping";
            continue;
        }

        if (split_points_.size() > 0 && split_points_.back() >= pitch) {
            OBJ_ERR << "ascending split point order expected, got: " << pitch << ", skipping";
            continue;
        }

        split_points_.push_back(pitch);
        char buf[64];
        sprintf(buf, "pitch: <%0.2g", pitch);
        split_tooltips_.push_back(buf);
        createOutlet();
    }

    createOutlet();

    createCbListProperty("@points", [this]() -> AtomList {
        AtomList res;
        res.reserve(split_points_.size());

        for (auto p : split_points_)
            res.append(p);

        return res;
    });
}

const char* MidiSplit::annotateOutlet(size_t n) const
{
    if (n >= split_tooltips_.size())
        return "list: non matched";

    return split_tooltips_[n].c_str();
}

void MidiSplit::onList(const AtomListView& lv)
{
    if (!onlist_chk->check(lv)) {
        OBJ_ERR << "PITCH VEL [DUR] expected, got: " << lv;
        return;
    }

    const auto pitch = lv[0].asT<t_float>();

    // optimization
    if (split_points_.empty() || pitch >= split_points_.back())
        return listTo(split_points_.size(), lv);

    for (size_t i = 0; i < split_points_.size(); i++) {
        if (pitch < split_points_[i])
            return listTo(i, lv);
    }
}

void setup_midi_split()
{
    onlist_chk.reset(new ArgChecker("f0..127 f0..127 f?"));

    ObjectFactory<MidiSplit> obj("midi.split");
    obj.setInletsInfo({ "list: PITCH VEL DUR?" });
}
