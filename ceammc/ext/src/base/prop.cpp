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
#include "prop.h"
#include "ceammc_canvas.h"
#include "ceammc_crc32.h"
#include "ceammc_factory.h"
#include "fmt/core.h"

extern "C" {
#include "g_canvas.h"
}

namespace {

CEAMMC_DEFINE_HASH(false);
CEAMMC_DEFINE_HASH(true);

inline t_symbol* makePropName(const AtomListView& lv)
{
    if (lv.isSymbol())
        return lv[0].asSymbol();
    else
        return gensym("@invalid");
}

}

BaseProp::BaseProp(const PdArgs& args)
    : BaseObject(args)
    , name_(makePropName(args.args))
    , full_name_(PropertyStorage::makeFullName(name_, canvas()))
{
    createOutlet();
    bindReceive(full_name_);
}

void BaseProp::parseProperties()
{
}

void BaseProp::onBang()
{
    PropertyPtr prop(full_name_);

    if (!prop) {
        OBJ_ERR << "property is not found: " << name_;
        return;
    }

    if (prop->isFloat()) {
        t_float f = 0;
        if (prop->getFloat(f))
            floatTo(0, f);
    } else if (prop->isInt()) {
        int i = 0;
        if (prop->getInt(i))
            floatTo(0, i);
    } else if (prop->isBool()) {
        bool b = false;
        if (prop->getBool(b))
            floatTo(0, b ? 1 : 0);
    } else if (prop->isSymbol()) {
        t_symbol* s;
        if (prop->getSymbol(&s))
            symbolTo(0, s);
    } else if (prop->isList()) {
        AtomList l;
        if (prop->getList(l))
            listTo(0, l);
    } else {
        OBJ_ERR << "unknown property type";
    }
}

void BaseProp::onFloat(t_float v)
{
    PropertyPtr prop(full_name_);

    if (!prop) {
        OBJ_ERR << "property is not found: " << name_;
        return;
    }

    if (prop->isFloat()) {
        prop->setFloat(v);
    } else if (prop->isInt()) {
        prop->setInt(v);
    } else if (prop->isBool()) {
        if (v != 0.0 && v != 1.0) {
            OBJ_ERR << "1 or 0 expected for bool property";
        } else
            prop->setBool(v);
    } else {
        OBJ_ERR << "not a float, int or bool property";
    }
}

void BaseProp::onSymbol(t_symbol* s)
{
    PropertyPtr prop(full_name_);

    if (!prop) {
        OBJ_ERR << "property is not found: " << name_;
        return;
    }

    if (prop->isSymbol()) {
        if (!prop->setSymbol(s))
            OBJ_ERR << "can't set property to " << s;
    } else if (prop->isBool()) {
        auto hash = crc32_hash(s);
        if (!(hash == hash_true || hash == hash_false))
            OBJ_ERR << fmt::format("only '{}' and '{}' values are supported, got: {}", str_true, str_false, s->s_name);
        else if (!prop->setBool(hash == hash_true))
            OBJ_ERR << "can't set property to " << s;
    } else
        OBJ_ERR << "not a symbol property";
}

void BaseProp::onList(const AtomListView& lv)
{
    PropertyPtr prop(full_name_);

    if (!prop) {
        OBJ_ERR << "property is not found: " << name_;
        return;
    }

    if (lv.isFloat() && (prop->isFloat() || prop->isInt() || prop->isBool()))
        return onFloat(lv[0].asFloat());

    if (lv.isSymbol() && (prop->isSymbol() || prop->isBool()))
        return onSymbol(lv[0].asSymbol());

    if (!prop->isList()) {
        OBJ_ERR << "not a list property";
        return;
    }

    if (!prop->setList(lv))
        OBJ_ERR << "can't set property to " << lv;
}

void BaseProp::m_default(t_symbol*, const AtomListView&)
{
    DataTypeProperty* prop = PropertyStorage::storage().acquire(full_name_);

    if (!prop) {
        OBJ_ERR << "property is not found: " << name_;
        return;
    }

    prop->restoreDefault();

    PropertyStorage::storage().release(full_name_);
}

void BaseProp::dump() const
{
    BaseObject::dump();

    DataTypeProperty* prop = PropertyStorage::storage().acquire(full_name_);

    if (!prop) {
        OBJ_ERR << "can't get property info: " << name_;
        return;
    }

    OBJ_DBG << "name:      " << name_->s_name << "\n"
            << "full name: " << full_name_ << "\n"
            << "type:      " << to_string(prop->propertyType()) << "\n"
            << "value:     " << prop->propertyStrValue();

    if (prop->hasMinValue())
        OBJ_DBG << "min:       " << prop->propertyStrMinValue();
    if (prop->hasMaxValue())
        OBJ_DBG << "max:       " << prop->propertyStrMaxValue();
    if (prop->hasEnumValues())
        OBJ_DBG << "enum:      " << prop->enumValues();

    PropertyStorage::storage().release(full_name_);
}

void setup_base_prop()
{
    ObjectFactory<BaseProp> obj("prop");
    obj.addMethod("default", &BaseProp::m_default);

    obj.setDescription("get/set named property value for subpatch or abstraction");
    obj.addAuthor("Serge Poltavsky");
    obj.setKeywords({ "property" });
    obj.setCategory("property");
    obj.setSinceVersion(0, 7);
}
