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
#include "canvas_prop.h"
#include "ceammc_canvas.h"
#include "ceammc_factory.h"

extern "C" {
#include "g_canvas.h"
}

class PropertyPtr {
    std::string name_;
    DataTypeProperty* prop_;

public:
    PropertyPtr(const std::string& name)
        : name_(name)
        , prop_(PropertyStorage::storage().acquire(name_))
    {
    }

    DataTypeProperty* operator->()
    {
        return prop_;
    }

    operator bool() const
    {
        return prop_ != nullptr;
    }

    ~PropertyPtr()
    {
        if (prop_)
            PropertyStorage::storage().release(name_);
    }
};

static t_symbol* SYM_INVALID = gensym("@invalid");

static t_symbol* makePropName(const AtomList& l)
{
    if (l.isSymbol())
        return l[0].asSymbol();
    else
        return SYM_INVALID;
}

CanvasProp::CanvasProp(const PdArgs& args)
    : BaseObject(args)
    , name_(makePropName(args.args))
    , full_name_(PropertyStorage::makeFullName(name_->s_name, canvas()))
{
    createOutlet();
    bindReceive(gensym(full_name_.c_str()));
}

void CanvasProp::parseProperties()
{
}

void CanvasProp::onBang()
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
        long i = 0;
        if (prop->getInt(i))
            floatTo(0, i);
    } else if (prop->isBool()) {
        bool b = false;
        if (prop->getBool(b))
            floatTo(0, b ? 1 : 0);
    } else {
        OBJ_ERR << "unknown property type";
    }
}

void CanvasProp::onFloat(t_float v)
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
        OBJ_ERR << "unknown property type";
    }
}

void CanvasProp::onSymbol(t_symbol* s)
{
    PropertyPtr prop(full_name_);

    if (!prop) {
        OBJ_ERR << "property is not found: " << name_;
        return;
    }

    if (prop->isSymbol()) {
        OBJ_ERR << "not a symbol property";
        return;
    }

    if (!prop->setSymbol(s))
        OBJ_ERR << "can't set property to " << s;
}

void CanvasProp::onList(const AtomList& l)
{
    PropertyPtr prop(full_name_);

    if (!prop) {
        OBJ_ERR << "property is not found: " << name_;
        return;
    }

    if (prop->isList()) {
        OBJ_ERR << "not a list property";
        return;
    }

    if (!prop->setList(l))
        OBJ_ERR << "can't set property to " << l;
}

void CanvasProp::m_default(t_symbol*, const AtomList&)
{
    DataTypeProperty* prop = PropertyStorage::storage().acquire(full_name_);

    if (!prop) {
        OBJ_ERR << "property is not found: " << name_;
        return;
    }

    prop->restoreDefault();

    PropertyStorage::storage().release(full_name_);
}

void setup_canvas_prop()
{
    ObjectFactory<CanvasProp> obj("prop");
    obj.addMethod("default", &CanvasProp::m_default);
}
