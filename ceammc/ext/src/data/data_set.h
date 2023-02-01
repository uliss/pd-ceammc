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
#ifndef DATA_SET_H
#define DATA_SET_H

#include "dataset_iface.h"
#include "datatype_set.h"
#include "editor_data.h"

using namespace ceammc;
using DataSetBase = EditorDataT<DataSetIface<BaseObject>, DataTypeSet>;

class DataSet : public DataSetBase {
    DataTypeSet set_;

public:
    DataSet(const PdArgs& a);

    DataTypeSet& data() final { return set_; }
    const DataTypeSet& data() const final { return set_; }

    EditorTitleString editorTitle() const final { return "data::Set"; }

    DataTypeSet& editorData() final { return set_; }
    const DataTypeSet& editorData() const final { return set_; }
};

void setup_data_set();

#endif // DATA_SET_H
