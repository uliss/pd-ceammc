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
#include "data_dict.h"

DataDict::DataDict(const PdArgs& args)
    : DataDictBase(args)
{
    setSpecialSymbolEscape(EDITOR_ESC_MODE_DATA);
    dict_->setFromDataList(args.args);

    createOutlet();
}

void setup_data_dict()
{
    DictIFaceFactory<DataDict> obj("data.dict");
    obj.addAlias("dict");
    obj.noArgsAndPropsParse();
    obj.setXletsInfo({ "bang: output\n"
                       "methods: add, clear, get_key, set_key, remove, set, read, write" },
        { "data: dict" });

    DataDict::registerMethods(obj);

    obj.setDescription("dictionary container: store values by key");
    obj.setCategory("data");
    obj.setKeywords({"data", "dictionary"});
}
