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
#include "local_set.h"
#include "ceammc_factory.h"

LocalSet::LocalSet(const PdArgs& a)
    : LocalSetBase(a)
{
    setSpecialSymbolEscape(EDITOR_ESC_MODE_DATA);
    createOutlet();
}

EditorTitleString LocalSet::editorTitle() const
{
    char buf[STATIC_STRING_SIZE(EditorTitleString)];
    snprintf(buf, sizeof(buf) - 1, "local Set (%s)", binbufArgs().symbolAt(0, gensym("default"))->s_name);
    return buf;
}

void setup_local_set()
{
    ColectionIFaceFactory<LocalSet> obj("local.set");
    obj.processData<DataTypeSet>();

    LocalSet::registerMethods(obj);

    obj.setDescription("local named set object");
    obj.setCategory("local");
    obj.setKeywords({"set", "local", "global"});
}
