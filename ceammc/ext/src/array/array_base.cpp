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
#include "array_base.h"

ArrayBase::ArrayBase(const PdArgs& a)
    : BaseObject(a)
    , name_(0)
{
    if (!a.args.empty()) {
        const Atom& first = a.args[0];
        if (first.isSymbol())
            name_ = first.asSymbol();
        else
            OBJ_ERR << "array name is not specified";
    }
}

t_garray* ArrayBase::findArray(t_symbol* s) const
{
    if (!s)
        return 0;

    t_garray* arr = reinterpret_cast<t_garray*>(pd_findbyclass(s, garray_class));
    if (!arr) {
        OBJ_ERR << "array not found: " << s->s_name;
        return 0;
    }

    int vecsize = 0;
    t_word* vecs;
    if (!garray_getfloatwords(arr, &vecsize, &vecs)) {
        OBJ_ERR << "invalid array template: " << s->s_name;
        return 0;
    }

    return arr;
}

long ArrayBase::arraySize() const
{
    if (!name_) {
        OBJ_ERR << "array name is not specified.";
        return -1;
    }

    // try to find array
    t_garray* a = findArray(name_);
    if (!a)
        return -1;

    int vecsize = 0;
    t_word* vecs;
    if (!garray_getfloatwords(a, &vecsize, &vecs)) {
        OBJ_ERR << "invalid array template: " << name_->s_name;
        return -1;
    }

    return vecsize;
}
