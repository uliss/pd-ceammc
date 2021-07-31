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
#ifndef PROTO_MIDI_CASIO_H
#define PROTO_MIDI_CASIO_H

#include "ceammc_object.h"
#include "ceammc_property_enum.h"

#include <array>
#include <cstdint>
#include <initializer_list>
#include <limits>

using namespace ceammc;

class ProtoMidiCasio : public BaseObject {
    SymbolEnumProperty* model_;

public:
    ProtoMidiCasio(const PdArgs& args);

    void m_reverb_type(t_symbol* s, const AtomListView& lv);
    void m_reverb_time(t_symbol* s, const AtomListView& lv);

private:
    template <typename T>
    bool output(const T& data)
    {
        if (data[0] == 0)
            return false;

        for (auto v : data)
            floatTo(0, v);

        return true;
    }
};

void setup_proto_midi_casio();

#endif // PROTO_MIDI_CASIO_H
