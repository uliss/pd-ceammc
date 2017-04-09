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

#include "m_pd.h"

#include "../mod_init.h"

t_symbol* any = &s_anything;
extern "C" void pd_init();

using namespace std;

int main(int, char* [])
{
    pd_init();
    ceammc_init();

    set<string> ceammc_set = ceammc_ext_list();
    copy(ceammc_set.begin(), ceammc_set.end(), ostream_iterator<string>(cout, "\n"));

    return EXIT_SUCCESS;
}
