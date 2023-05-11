/*****************************************************************************
 * Copyright 2023 Serge Poltavski. All rights reserved.
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
#ifndef DATA_BIMAP_H
#define DATA_BIMAP_H

#include "ceammc_editor_object.h"
#include "ceammc_object.h"
#include "editor_list.h"
#include "list_iface.h"

#include <boost/bimap.hpp>

using namespace ceammc;

using BiMap = boost::bimap<Atom, Atom>;

class DataBiMap : public EditorListT<DataListIFace<BaseObject>> {
    BiMap bimap_;
    ListProperty* values_ { 0 };

public:
    DataBiMap(const PdArgs& args);

    void onFloat(t_float v) final;
    void onSymbol(t_symbol* s) final;
    void onList(const AtomListView& lv) final;
    void onInlet(size_t idx, const AtomListView& lv) final;

    EditorTitleString editorTitle() const final { return "DATA.BIMAP"; }

    const AtomList& list() const { return values_->value(); }
    AtomList& list() { return values_->value(); }
};

void setup_data_bimap();

#endif // DATA_BIMAP_H
