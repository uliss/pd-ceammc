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

#include "m_imp.h"

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

namespace pd {

    bool atoms_minmax(const atom_list& lst, t_float* min, t_float* max)
    {
        assert(min);
        assert(max);

        if (lst.empty())
            return false;

        t_float a_min = std::numeric_limits<t_float>::max();
        t_float a_max = -std::numeric_limits<t_float>::max();
        bool found = false;

        for (size_t i = 0; i < lst.size(); i++) {
            t_float v;
            if (get_float(lst[i], &v)) {
                a_min = std::min(a_min, v);
                a_max = std::max(a_max, v);
                found = true;
            }
        }

        if (!found)
            return false;

        *min = a_min;
        *max = a_max;
        return true;
    }

    bool atoms_normalize(atom_list& lst)
    {
        if (lst.empty())
            return false;

        // find min/max
        t_float a_min = 0;
        t_float a_max = 0;
        if (!atoms_minmax(lst, &a_min, &a_max))
            return false;

        // if equal values - set all to 1
        if (a_max == a_min) {
            for (size_t i = 0; i < lst.size(); i++) {
                if (is_float(lst[i])) {
                    SETFLOAT(&lst[i], 1);
                }
            }
        } else {
            for (size_t i = 0; i < lst.size(); i++) {
                t_float v;
                if (get_float(lst[i], &v)) {
                    t_float n = (v - a_min) / (a_max - a_min);
                    SETFLOAT(&lst[i], n);
                }
            }
        }
        return true;
    }
}
}
