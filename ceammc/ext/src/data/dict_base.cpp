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
#include "dict_base.h"
#include "ceammc_format.h"
#include "datatype_dict.h"

DictBase::DictBase(const PdArgs& args)
    : BaseObject(args)
{
}

void DictBase::onAny(t_symbol* s, const AtomListView& lst)
{
    if (s->s_name[0] != '[')
        return BaseObject::onAny(s, lst);

    std::string str = s->s_name;
    if (!lst.empty()) {
        str.push_back(' ');
        str += to_string(lst, " ");
    }

    DictAtom dict;
    if (dict->fromString(str))
        onDataT(dict);
    else
        LIB_ERR << "not a dict: " << str;
}
