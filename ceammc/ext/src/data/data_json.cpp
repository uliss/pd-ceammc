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
#include "ceammc_datatypes.h"
#include "ceammc_factory.h"
#include "ceammc_format.h"
#include "ceammc_log.h"

DataJson::DataJson(const PdArgs& args)
    : CollectionIFace<BaseObject>(args)
    , json_(nullptr)
{
    createOutlet();

    auto str = to_string(args.args, " ");
    if (!str.empty())
        json_ = JsonPtr(new DataTypeTree(str.c_str()));
    else
        json_ = JsonPtr(new DataTypeTree());
}

void DataJson::proto_add(const AtomList& lst)
{
    if (json_->isArray() || json_->isNull()) {
        auto p = json_->cloneT<DataTypeTree>();
        JsonPtr j(p);

        if (lst.isFloat()) {
            auto f = atomlistToValue<t_float>(lst, 0);
            if (!p->addFloat(f))
                OBJ_ERR << "can't add float to json: " << f;
            else
                json_ = j;
        } else if (lst.isSymbol()) {
            auto s = atomlistToValue<t_symbol*>(lst, &s_);
            if (!p->addSymbol(s))
                OBJ_ERR << "can't add symbol to json: " << s;
            else
                json_ = j;
        } else if (lst.isList() && lst.allOf(isFloat)) {
            if (!p->addList(lst))
                OBJ_ERR << "can't add list to json: " << lst;
            else
                json_ = j;
        } else if (lst.isDataType(data::DATA_TREE)) {
            JsonPtr jptr(lst[0]);
            if (jptr.isNull()) {
                OBJ_ERR << "invalid json data: " << lst;
                return;
            }

            if (jptr->isNull())
                return;

            if (!p->addJson(*jptr))
                OBJ_ERR << "can't add json to json: " << lst;
            else
                json_ = j;
        } else {
            auto str = to_string(lst, " ");
            if (!p->addJson(DataTypeTree(str.c_str())))
                OBJ_ERR << "can't add json to json: " << lst;
            else
                json_ = j;
        }
    }
}

bool DataJson::proto_remove(const AtomList& lst)
{
}

void DataJson::proto_set(const AtomList& lst)
{
    if (lst.isFloat())
        json_ = JsonPtr(new DataTypeTree(atomlistToValue<t_float>(lst, 0)));
    else if (lst.isSymbol())
        json_ = JsonPtr(new DataTypeTree(atomlistToValue<t_symbol*>(lst, &s_)));
    else if (lst.isDataType(data::DATA_TREE))
        json_ = JsonPtr(lst[0]);
    else if (lst.allOf(isFloat))
        json_ = JsonPtr(new DataTypeTree(lst.asFloats()));
    else {
        auto str = to_string(lst, " ");
        json_ = JsonPtr(new DataTypeTree(str.c_str()));
    }
}

void DataJson::proto_clear()
{
    if (!json_->isNull())
        json_ = JsonPtr(new DataTypeTree);
}

size_t DataJson::proto_size() const
{
    return json_->size();
}

void DataJson::onBang()
{
    this->dataTo(0, json_);
}

void DataJson::dump() const
{
    CollectionIFace<BaseObject>::dump();
    OBJ_DBG << json_->toString();
}

void DataJson::onDataT(const DataTPtr<DataTypeTree>& j)
{
    json_ = j;
    dataTo(0, j);
}

void DataJson::m_find(t_symbol* s, const AtomList& l)
{
    dataTo(0, json_->match(to_string(l).c_str()));
}

void DataJson::m_at(t_symbol* s, const AtomList& l)
{
    if (l.isFloat()) {
        auto f = atomlistToValue<t_float>(l, 0);
        if (f < 0) {
            METHOD_ERR(s) << "invalid index: " << f;
            return;
        }

        dataTo(0, json_->at(f));
    } else if (l.isSymbol()) {
        dataTo(0, json_->at(atomlistToValue<t_symbol*>(l, 0)));
    }
}

void DataJson::m_key(t_symbol* s, const AtomList& l)
{
    //    dataTo(0, json_->key());
}

void DataJson::m_insert(t_symbol* s, const AtomList& lst)
{
    DataTypeTree* p = json_->cloneT<DataTypeTree>();
    JsonPtr j(p);

    if (lst.size() < 1 || !lst[0].isSymbol()) {
        METHOD_ERR(s) << "usage: " << s->s_name << " KEY [VALUES]";
        return;
    }

    const char* key = lst[0].asSymbol()->s_name;

    if (lst.size() < 2) {

    } else {
        if (lst[1].isFloat()) {
            auto f = lst[1].asFloat();
            if (!p->insertFloat(key, f))
                OBJ_ERR << "can't insert float to json: " << key << " " << f;
            else
                json_ = j;
        } else if (lst[1].isSymbol()) {
            auto s = lst[1].asSymbol();
            if (!p->insertSymbol(key, s))
                OBJ_ERR << "can't insert symbol to json: " << key << " " << s;
            else
                json_ = j;
        } /*else if (lst.isList() && lst.allOf(isFloat)) {
        if (!p->insert(lst))
            OBJ_ERR << "can't add list to json: " << lst;
        else
            json_ = j;
    }*/
    }
}

void setup_data_json()
{
    ColectionIFaceFactory<DataJson> obj("data.json");
    obj.processData<DataTypeTree>();
    obj.addMethod("at", &DataJson::m_at);
    obj.addMethod("find", &DataJson::m_find);
    obj.addMethod("insert", &DataJson::m_insert);
}
