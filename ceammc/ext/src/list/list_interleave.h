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
#ifndef LIST_HISTOGRAM_H
#define LIST_HISTOGRAM_H

#include "ceammc_object.h"

using namespace ceammc;

class ListInterleave : public BaseObject {
    const size_t in_count_;
    std::vector<AtomList> in_list_;
    AtomList out_list_;
    SymbolEnumProperty* method_;
    Atom pad_;
    t_symbol* gmin_;
    t_symbol* gpad_;
    t_symbol* gclip_;
    t_symbol* gwrap_;
    t_symbol* gfold_;

public:
    ListInterleave(const PdArgs& a);

    void onBang();
    void onList(const AtomList& l);
    void onInlet(size_t n, const AtomList& l);
    void dump() const;

private:
    void initInlets();
    void initLists();
    void initProperties();
    AtomList getPadValue() const;
    void setPadValue(const AtomList& l);
};


#endif
