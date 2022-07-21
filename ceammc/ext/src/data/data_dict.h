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
#ifndef DATA_DICT_H
#define DATA_DICT_H

#include "dict_iface.h"
#include "editor_data.h"

using namespace ceammc;

using DataDictBase = EditorDataT<DictIFace<BaseObject>, DataTypeDict>;

class DataDict : public DataDictBase {
    DictAtom dict_;

public:
    DataDict(const PdArgs& args);
    const DictAtom& dict() const final { return dict_; }
    DictAtom& dict() final { return dict_; }

    const DataTypeDict& editorData() const final { return *dict_; }
    DataTypeDict& editorData() final { return *dict_; }

    EditorTitleString editorTitle() const final { return "Dict"; }
};

void setup_data_dict();

#endif // DATA_DICT_H
