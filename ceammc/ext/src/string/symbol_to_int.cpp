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
#include "symbol_to_int.h"
#include "ceammc_containers.h"
#include "ceammc_factory.h"
#include "ceammc_string.h"

#include "utf8rewind/utf8rewind.h"

#include <cstdint>

SymbolToInt::SymbolToInt(const PdArgs& args)
    : BaseObject(args)
{
    createOutlet();
}

void SymbolToInt::onSymbol(t_symbol* s)
{
    const size_t N = string::utf8_strlen(s->s_name);
    if (N == 0)
        return;

    AtomList256 out;
    out.assign(N, Atom());
    size_t out_idx = 0;

    const char* c0 = s->s_name;
    const char* c1 = s->s_name;
    const size_t NBytes = strlen(s->s_name);
    int len = N;

    while (len-- > 0) {
        c1 = utf8seek(c0, NBytes, s->s_name, 1, SEEK_CUR);
        const size_t char_len = c1 - c0;

        switch (char_len) {
        case 0:
            goto end;
            break;
        case 1:
            out[out_idx++].setFloat(c0[0], true);
            break;
        case 2:
            out[out_idx++].setFloat(uint8_t(c0[1]) | (uint8_t(c0[0]) << 8), true);
            break;
        case 3:
            out[out_idx++].setFloat(uint8_t(c0[2]) | (uint8_t(c0[1]) << 8) | (uint8_t(c0[0]) << 16), true);
            break;
        case 4: {
            const auto b0 = uint8_t(c0[3]);
            const auto b1 = uint32_t(uint8_t(c0[2])) << 8;
            const auto b2 = uint32_t(uint8_t(c0[1])) << 16;
            const auto b3 = uint32_t(uint8_t(c0[0]) << 24);
            const auto v = b0 | b1 | b2 | b3;

            out[out_idx++].setFloat(v, true);
        } break;
        default:
            goto end;
            break;
        }

        c0 = c1;
    }

end:;

    listTo(0, AtomListView(&out[0].atom(), N));
}

void setup_symbol_to_int()
{
    ObjectFactory<SymbolToInt> obj("symbol2intlist");
    obj.addAlias("sym->ilist");
}
