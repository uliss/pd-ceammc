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
#include "ceammc.h"
#include "ceammc_atomlist.h"
#include "ceammc_pd.h"
#include "stk/stk/include/Stk.h"

#include <algorithm>
#include <cstdlib>
#include <iostream>
#include <string>
#include <vector>

#ifndef STK_RAWWAVES
#define STK_RAWWAVES "stk/rawwaves"
#endif

using namespace ceammc;

extern "C" CEAMMC_EXTERN bool ceammc_list_methods(int argc, char* argv[])
{
    CanvasPtr cnv = PureData::instance().createTopCanvas("/test_canvas");

    if (!cnv) {
        std::cerr << "can't create root canvas" << std::endl;
        return false;
    }

    AtomList args;
    if (argc > 1) {
        t_binbuf* b = binbuf_new();

        std::string str;
        for (int i = 1; i < argc; i++) {
            str += argv[i];
            str += " ";
        }

        binbuf_text(b, str.c_str(), str.size());

        int n = binbuf_getnatom(b);

        for (int i = 0; i < n; i++)
            args.append(binbuf_getvec(b)[i]);

        binbuf_free(b);
    }

    // stk rawwaves path
    if (getenv("RAWWAVES") != nullptr)
        stk::Stk::setRawwavePath(getenv("RAWWAVES"));
    else
        stk::Stk::setRawwavePath(STK_RAWWAVES);

    pd::External ext(argv[0], args);
    if (!ext.object()) {
        std::cerr << "can't create object: " << argv[0] << std::endl;
        return false;
    }

    std::vector<std::string> m_vec;
    for (auto& m : ext.methods())
        m_vec.push_back(m->s_name);

    std::sort(begin(m_vec), end(m_vec));

    for (auto& m : m_vec)
        std::cout << m << "\n";

    return true;
}
