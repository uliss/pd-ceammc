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
#include "conv_char2morse.h"
#include "ceammc_containers.h"
#include "ceammc_factory.h"
#include "ceammc_string.h"
#include "gperf_morse.h"

ConvChar2Morse::ConvChar2Morse(const PdArgs& args)
    : BaseObject(args)
    , sym_(nullptr)
{
    sym_ = new BoolProperty("@sym", false);
    addProperty(sym_);

    createOutlet();
}

void ConvChar2Morse::onSymbol(t_symbol* s)
{
    auto str = s->s_name;
    auto next_char = string::utf8_next(str, strlen(str));
    if (next_char == str) {
        OBJ_ERR << "can't seek string";
        return;
    }

    auto hash = MorsePerfectHash::in_word_set(str, next_char - str);
    if (!hash) {
        OBJ_LOG << "can't find code for symbol: " << str;
        return;
    }

    StaticAtomList<6> out;
    if (sym_->value()) { // symbol mode
        Atom morse[2] = { gensym("."), gensym("-") };
        for (int i = 0; i < hash->code.length; i++) {
            auto c = hash->code.at(i);
            if (c >= 0)
                out.push_back(morse[c]);
        }
    } else { // bin
        for (int i = 0; i < hash->code.length; i++)
            out.push_back(hash->code.at(i));
    }

    listTo(0, out.view());
}

void setup_conv_char2morse()
{
    ObjectFactory<ConvChar2Morse> obj("conv.char2morse");
    obj.addAlias("char->morse");
}
