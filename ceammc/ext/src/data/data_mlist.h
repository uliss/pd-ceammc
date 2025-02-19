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
#ifndef DATA_MLIST_H
#define DATA_MLIST_H

#include "ceammc_data.h"
#include "ceammc_property_data.h"
#include "editor_data.h"
#include "mlist_iface.h"

using DataMListBase = EditorDataT<DataMListIFace<BaseObject>, DataTypeMList>;
using MListProperty = DataPropertyT<DataTypeMList>;

class DataMList : public DataMListBase {
    MListAtom mlist_;
    MListProperty* value_ { nullptr };

public:
    DataMList(const PdArgs& args);

    const DataTypeMList& editorData() const final { return value_->value(); }
    DataTypeMList& editorData() final { return value_->value(); }

    MListAtom& mlist() final { return value_->atomRef(); }
    const MListAtom& mlist() const final { return value_->atomRef(); }

    EditorTitleString editorTitle() const final { return "MList"; }
};

void setup_data_mlist();

#endif // DATA_MLIST_H
