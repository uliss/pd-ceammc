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
#ifndef GLOBAL_DICT_H
#define GLOBAL_DICT_H

#include "dict_iface.h"
#include "editor_data.h"
#include "global_base.h"

using namespace ceammc;

using GlobalDictIFace = DictIFace<GlobalBase<DictAtom>>;
using GlobalDictBase = EditorDataT<GlobalDictIFace, DataTypeDict>;

class GlobalDict : public GlobalDictBase {
public:
    GlobalDict(const PdArgs& args);
    const DictAtom& dict() const final { return ref(); }
    DictAtom& dict() final { return ref(); }

    const DataTypeDict& editorData() const final { return *dict(); }
    DataTypeDict& editorData() final { return *dict(); }

    EditorTitleString editorTitle() const final;
};

void setup_global_dict();

#endif // GLOBAL_DICT_H
