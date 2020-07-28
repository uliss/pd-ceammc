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
#include "conv_hex2int.h"
#include "ceammc_factory.h"

#include <cstdlib>

ConvHex2Int::ConvHex2Int(const PdArgs& args)
    : BaseObject(args)
    , is_strict(nullptr)
{
    createOutlet();

    is_strict = new FlagProperty("@strict");
    addProperty(is_strict);
}

void ConvHex2Int::onSymbol(t_symbol* s)
{
    char* end = nullptr;
    auto v = std::strtol(s->s_name, &end, 16);
    if (v == 0 && *end != 0) { // not full match
        OBJ_ERR << "symbol in hex format ([+-]?(0x)?[0-9a-fA-F]+)) expected, got: " << s;
        return;
    }

    floatTo(0, v);
}

void ConvHex2Int::onList(const AtomList& l)
{
    AtomList res;
    res.reserve(l.size());
    for (auto& a : l) {
        t_symbol* s = a.asSymbol(nullptr);
        if (s) {
            char* end = nullptr;
            auto v = std::strtol(s->s_name, &end, 16);
            if (v == 0 && *end != 0) { // not full match
                if (is_strict->value()) { // discard in strict mode
                    OBJ_ERR << "symbol in hex format ([+-]?(0x)?[0-9a-fA-F]+)) expected, got: " << a;
                    continue;
                } else
                    res.append(a); // append as is

            } else
                res.append(Atom(v)); // append converted value
        } else if (is_strict->value())
            OBJ_ERR << "symbol in hex format expected, got: " << a;
        else
            res.append(a);
    }

    listTo(0, res);
}

void setup_conv_hex2int()
{
    ObjectFactory<ConvHex2Int> obj("conv.hex2int");
    obj.addAlias("hex->int");
    obj.setInletsInfo({ "symbol or list of hex values" });
    obj.setOutletsInfo({ "int or list of ints" });
}
