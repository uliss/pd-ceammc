/*****************************************************************************
 * Copyright 2020 Serge Poltavsky. All rights reserved.
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
#include "data_json.h"
#include "ceammc_factory.h"
#include "ceammc_format.h"
#include "ceammc_log.h"

DataJson::DataJson(const PdArgs& args)
    : CollectionIFace<BaseObject>(args)
{
    createOutlet();
    auto str = to_string(args.args, " ");
    if (!str.empty())
        json_.parse(str.c_str());
}

void DataJson::proto_add(const AtomList& lst)
{
    if (json_.isArray() || json_.isNull()) {
        if (lst.isFloat()) {
            auto f = atomlistToValue<t_float>(lst, 0);
            if (!json_.addFloat(f))
                OBJ_ERR << "can't add float to json: " << f;
        } else if (lst.isSymbol()) {
            auto s = atomlistToValue<t_symbol*>(lst, &s_);
            if (!json_.addSymbol(s))
                OBJ_ERR << "can't add symbol to json: " << s;
        } else if (lst.isList()) {
            if (!json_.addList(lst))
                OBJ_ERR << "can't add list to json: " << lst;
        }
    }
}

bool DataJson::proto_remove(const AtomList& lst)
{
}

void DataJson::proto_set(const AtomList& lst)
{
    json_.parse(to_string(lst, " ").c_str());
}

void DataJson::proto_clear()
{
    json_.clear();
}

size_t DataJson::proto_size() const
{
    return json_.size();
}

void DataJson::onBang()
{
    this->dataTo(0, json_.clone());
}

void DataJson::dump() const
{
    CollectionIFace<BaseObject>::dump();
    OBJ_DBG << json_.toString();
}

void setup_data_json()
{
    ColectionIFaceFactory<DataJson> obj("data.json");
    //    obj.processData<DataTypeMList>();
    //    ObjectFactory<DataJson> obj("data.json");
}
