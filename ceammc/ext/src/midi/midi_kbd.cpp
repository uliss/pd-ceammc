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
#include "midi_kbd.h"
#include "ceammc_factory.h"
#include "kbd_names.h"
#include "kbd_querty.h"

#include <limits>

constexpr auto KEY_NOT_FOUND = std::numeric_limits<int>::max();

static t_symbol* SYM_KEYUP;
static t_symbol* SYM_KEYDOWN;

MidiKbd::MidiKbd(const PdArgs& args)
    : BaseObject(args)
    , keypress_(this, &MidiKbd::onKeyPress)
    , keyrelease_(this, &MidiKbd::onKeyRelease)
    , octave_(nullptr)
    , vel_(nullptr)
    , kbd_(&kbd_querty)
{
    keypress_.bind(SYM_KEYDOWN);
    keyrelease_.bind(SYM_KEYUP);

    octave_ = new IntProperty("@octave", 4);
    octave_->checkClosedRange(0, 8);
    addProperty(octave_);

    vel_ = new FloatProperty("@vel", 100);
    vel_->checkClosedRange(0, 127);
    addProperty(vel_);

    createOutlet();
}

int MidiKbd::findNote(int key) const
{
    if (!kbd_)
        return KEY_NOT_FOUND;

    auto it = kbd_->find(key);
    return (it == kbd_->end()) ? KEY_NOT_FOUND : it->second;
}

void MidiKbd::onKeyPress(t_float key)
{
    auto note = findNote(key);
    if (note == KEY_NOT_FOUND)
        return;

    Atom res[2] = { note + octave_->value() * 12, vel_->value() };
    listTo(0, AtomListView(res, 2));
}

void MidiKbd::onKeyRelease(t_float key)
{
    auto note = findNote(key);
    if (note == KEY_NOT_FOUND)
        return;

    Atom res[2] = { note + octave_->value() * 12, 0. };
    listTo(0, AtomListView(res, 2));
}

void setup_midi_kbd()
{
    SYM_KEYUP = gensym("#keyup");
    SYM_KEYDOWN = gensym("#key");
    ObjectFactory<MidiKbd> obj("midi.kbd");
}
