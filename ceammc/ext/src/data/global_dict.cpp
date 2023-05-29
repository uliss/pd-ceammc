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
#include "global_dict.h"
#include "ceammc_factory.h"

GlobalDict::GlobalDict(const PdArgs& args)
    : GlobalDictBase(args)
{
    setSpecialSymbolEscape(EDITOR_ESC_MODE_DATA);
    createOutlet();
}

EditorTitleString GlobalDict::editorTitle() const
{
    return makeEditorTitleString("global Dict", id()->s_name);
}

void setup_global_dict()
{
    DictIFaceFactory<GlobalDict> obj("global.dict");
    obj.addAlias("global.json");

    obj.setXletsInfo({ "bang: output\n"
                       "methods: add, clear, get_key, set_key, remove, set, read, write" },
        { "data: dict" });

    GlobalDict::registerMethods(obj);

    obj.setDescription("global named dict object");
    obj.setCategory("global");
    obj.setKeywords({"dict", "local", "global"});
}
