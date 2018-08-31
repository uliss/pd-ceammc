/*****************************************************************************
 *
 * Created: 04.12.2016 2016 by Serge Poltavski
 *
 * Copyright 2016 Serge Poltavski. All rights reserved.
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

#include "ceammc.hpp"
#include "config.h"

extern "C" {
#include "m_imp.h"
}

#include "ceammc_format.h"
#include "ceammc_pd.h"

#include <algorithm>
#include <cassert>
#include <cstdlib>
#include <cstring>
#include <limits>

namespace ceammc {

std::vector<std::string> currentExtensionList()
{
    std::vector<std::string> res;
    t_methodentry* m = pd_objectmaker->c_methods;
    if (!m)
        return res;

    for (int i = 0; i < pd_objectmaker->c_nmethod; i++)
        res.push_back(m[i].me_name->s_name);

    return res;
}

std::vector<t_symbol*>& objectExternalsList()
{
    static std::vector<t_symbol*> lst;
    return lst;
}

std::string get_env(const char* varname)
{
    std::string res;
    char* env = ::getenv(varname);
    if (env)
        res = env;

    return res;
}

void set_env(const char* varname, const char* val)
{
#ifdef HAVE_SETENV
    ::setenv(varname, val, 1);
#else
    std::string str(varname);
    str += '=';
    str += val;
    ::putenv(str.c_str());
#endif
}

static t_listmethod old_print_mlist = nullptr;

static void print_list_replace(t_pd* pd, t_symbol* s, int argc, t_atom* argv)
{
    if (!old_print_mlist)
        return;

    bool contains_data = false;
    for (int i = 0; i < argc; i++) {
        if (Atom(argv[i]).isData()) {
            contains_data = true;
            break;
        }
    }

    if (contains_data) {
        post("[data] %s", to_string(AtomList(argc, argv)).c_str());
    } else
        old_print_mlist(pd, s, argc, argv);
}

bool addPdPrintDataSupport()
{
    pd::External p("print");
    if (!p.object())
        return false;

    t_class* print_class = p.object()->te_g.g_pd;
    if (!print_class)
        return false;

    // save old callback
    old_print_mlist = print_class->c_listmethod;
    print_class->c_listmethod = print_list_replace;
    return true;
}
}
