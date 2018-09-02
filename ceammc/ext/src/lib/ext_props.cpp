/*****************************************************************************
 * Copyright 2018 Serge Poltavsky. All rights reserved.
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

#include "../mod_init.h"
#include "ceammc_object.h"
#include "ceammc_pd.h"
#include "ceammc_platform.h"
#include "m_pd.h"

#include <algorithm>
#include <iostream>
#include <string>
#include <vector>

static t_symbol* any = &s_anything;
extern "C" void pd_init();

using namespace std;
using namespace ceammc;

int main(int argc, char* argv[])
{
    pd_init();
    ceammc_init();

    if (argc < 2) {
        cerr << "usage: " << platform::basename(argv[0]) << " OBJECT_NAME" << endl;
        return 1;
    }

    CanvasPtr cnv = PureData::instance().createTopCanvas("/test_canvas");

    if (!cnv) {
        cerr << "can't create root canvas" << endl;
        return 2;
    }

    pd::External ext(argv[1], AtomList());
    if (!ext.object()) {
        cerr << "can't create object: " << argv[1] << endl;
        return 3;
    }

    if (!ext.isCeammc()) {
        cerr << "not a CEAMMC object: " << argv[1] << endl;
        return 4;
    }

    if (ext.isCeammcBase()) {
        BaseObject* obj = ext.asCeammcBaseObject();
        if (!obj) {
            cerr << "corrupted CEAMMC object";
            return 5;
        }

        for (auto& p : obj->properties()) {
            cout << p.first->s_name << endl;
        }
    }

    return 0;
}
