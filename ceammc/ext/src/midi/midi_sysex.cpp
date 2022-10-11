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
#include "midi_sysex.h"
#include "ceammc_containers.h"
#include "ceammc_factory.h"

constexpr const char* STR_SYSEXIN = "#sysexin";

constexpr uint8_t SYSEX_BEGIN = 0xF0;
constexpr uint8_t SYSEX_END = 0xF7;
constexpr size_t MAX_SYSEX = 1024 * 16;

static inline bool is_realtime(uint8_t byte)
{
    return byte > SYSEX_END;
}

MidiSysex::MidiSysex(const PdArgs& args)
    : BaseObject(args)
    , proxy_(this, &MidiSysex::onSysex)
    , state_(NOT_SYSEX)
{
    createOutlet();

    proxy_.bind(STR_SYSEXIN);
    buffer_.reserve(256);
}

void MidiSysex::onSysex(const AtomListView& lv)
{
    const uint8_t byte = lv.intAt(0, 0);

    // not sure it is really needed for pd
    if (is_realtime(byte))
        return;

    if (state_ == NOT_SYSEX) {
        if (byte == SYSEX_BEGIN) {
            buffer_.clear();
            buffer_.push_back(byte);
            state_ = IS_SYSEX;
        } else
            return; // ignore
    } else {
        if (byte >= SYSEX_BEGIN && byte <= SYSEX_END) {
            buffer_.push_back(byte);
            output();
            state_ = NOT_SYSEX;
            buffer_.clear();
        } else if (buffer_.size() == MAX_SYSEX) {
            OBJ_ERR << "sysex message is too big: " << buffer_.size() << ", rejecting";
            buffer_.clear();
            state_ = NOT_SYSEX;
        } else {
            buffer_.push_back(byte);
        }
    }
}

void MidiSysex::m_reset(t_symbol*, const AtomListView&)
{
    state_ = NOT_SYSEX;
    buffer_.clear();
}

void MidiSysex::output()
{
    const auto N = buffer_.size();
    AtomList512 b;
    b.reserve(N);

    for (size_t i = 0; i < N; i++)
        b.push_back(buffer_[i]);

    listTo(0, b.view());
}

void setup_midi_sysex()
{
    ObjectFactory<MidiSysex> obj("midi.sysex");
    obj.addMethod("reset", &MidiSysex::m_reset);

    obj.setXletsInfo({ "reset: reset parser" }, { "list: sysex bytes" });
}
