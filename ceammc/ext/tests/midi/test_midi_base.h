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
#ifndef TEST_MIDI_BASE_H
#define TEST_MIDI_BASE_H

#include "ceammc_data.h"
#include "datatype_dict.h"
#include "datatype_midistream.h"
#include "datatype_miditrack.h"
#include "datatype_string.h"
#include "midi_common.h"

#include "test_base.h"
#include "test_external.h"

#include "test_catch2.hpp"

using namespace ceammc;

using AT = Atom;
using AL = AtomList;

using DictA = DictAtom;
using StrA = StringAtom;
using MLA = MListAtom;

using IntA = DataAtom<IntData>;

using Msg = std::pair<t_symbol*, AtomList>;

/** msg */
template <typename T>
Msg msgAt(const T& t, OutletIdx idx = 0_out)
{
    if (!t.hasNewMessages(idx.n))
        throw NoOutput(idx);

    auto msg = t.lastMessage(idx.n);

    if (!msg.isAny())
        throw NotAny(to_string(msg));

    return std::make_pair(msg.atomValue().asSymbol(), msg.listValue());
}

#endif // TEST_MIDI_BASE_H
