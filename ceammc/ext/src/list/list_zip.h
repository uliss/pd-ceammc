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
#ifndef LIST_ZIP_H
#define LIST_ZIP_H

#include "ceammc_object.h"
#include "ceammc_property_enum.h"

using namespace ceammc;

class ListZip : public BaseObject {
    IntProperty* n_;
    SymbolEnumProperty* method_;
    std::vector<AtomList> in_list_;
    AtomList out_list_;
    Atom pad_;

public:
    ListZip(const PdArgs& a);
    void initDone() override;

    void onBang() override;
    void onList(const AtomListView& lv) override;
    void onInlet(size_t n, const AtomListView& l) override;
    void dump() const override;

private:
    void initInlets();
    void initLists();
};

void setup_list_zip();

#endif
