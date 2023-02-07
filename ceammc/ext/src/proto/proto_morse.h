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
#ifndef PROTO_MORSE_H
#define PROTO_MORSE_H

#include "ceammc_clock.h"
#include "ceammc_object.h"

#include <deque>
using namespace ceammc;

class ProtoMorse : public BaseObject {
    FloatProperty* dot_space_;
    IntProperty* word_space_;
    IntProperty* char_space_;
    BoolProperty* ext_clock_;
    ClockLambdaFunction clock_;
    std::uint8_t last_bit_;

    enum Code {
        CODE_DOT,
        CODE_DASH,
        CODE_SPACE_DOT,
        CODE_SPACE_CHAR,
        CODE_SPACE_WORD,
    };

    std::deque<std::uint8_t> queue_;

public:
    ProtoMorse(const PdArgs& args);

    void onSymbol(t_symbol* s) override;
    void onList(const AtomListView& lv) override;
    void onInlet(size_t n, const AtomListView& lv) override;

    void m_reset(t_symbol* s, const AtomListView& lv);

private:
    void processWord(const char* str);
    bool popBit();
};

void setup_proto_morse();

#endif // PROTO_MORSE_H
