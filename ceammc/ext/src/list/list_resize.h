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
#ifndef LIST_RESIZE_H
#define LIST_RESIZE_H

#include "ceammc_object.h"
#include "ceammc_property_enum.h"

using namespace ceammc;

class ListResize : public BaseObject {
    SizeTProperty* size_;
    SymbolEnumProperty* method_;
    Atom pad_;

public:
    ListResize(const PdArgs& a);
    void onInlet(size_t n, const AtomList& l) override;
    void onList(const AtomList& l) override;

private:
    void initProperties();
};

void setup_list_resize();

#endif // LIST_RESIZE_H
