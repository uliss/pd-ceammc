/*****************************************************************************
 * Copyright 2017 Serge Poltavsky. All rights reserved.
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

#include <algorithm>
#include <cstdlib>
#include <iostream>
#include <iterator>
#include <set>
#include <string>
#include <vector>

#include "m_pd.h"

#include "../mod_init.h"
#include "ceammc_pd.h"

static t_symbol* any = &s_anything;
extern "C" void pd_init();
t_class* ceammc_class = nullptr;

using namespace std;

int main(int argc, char* argv[])
{
    pd_init();

    bool vanilla = argc > 1 && string(argv[1]) == "-v";
    if (vanilla) {
        vector<string> l = ceammc::pd::currentListOfExternals();
        set<string> vanilla_set(l.begin(), l.end());
        copy(vanilla_set.begin(), vanilla_set.end(), ostream_iterator<string>(cout, "\n"));
        return EXIT_SUCCESS;
    }

    ceammc_init();

    set<string> ceammc_set = ceammc_ext_list();
    copy(ceammc_set.begin(), ceammc_set.end(), ostream_iterator<string>(cout, "\n"));

    return EXIT_SUCCESS;
}
