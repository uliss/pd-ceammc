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
#include "local_mlist.h"
#include "ceammc_factory.h"
#include "data_protocol.h"
#include "datatype_mlist.h"

LocalMList::LocalMList(const PdArgs& args)
    : LocalMListBase(args)
{
    setSpecialSymbolEscape(EDITOR_ESC_MODE_DATA);
    createOutlet();
}

EditorTitleString LocalMList::editorTitle() const
{
    return makeEditorTitleString("local MList", binbufArgs().symbolAt(0, gensym("default"))->s_name);
}

void setup_local_mlist()
{
    ListIFaceFactory<LocalMList> obj("local.mlist");
    obj.processData<DataTypeMList>();
    obj.useDefaultPdFloatFn();
    obj.useDefaultPdSymbolFn();

    LocalMList::registerMethods(obj);

    obj.setDescription("local named mlist object");
    obj.setCategory("local");
    obj.setKeywords({"mlist", "local", "global"});
}
