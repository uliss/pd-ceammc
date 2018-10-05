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
#include "ceammc_factory.h"

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
}

void CanvasProp::parseProperties()
{
}

void CanvasProp::onBang()
{
    DataTypeProperty* prop = PropertyStorage::storage().acquire(full_name_);

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

    PropertyStorage::storage().release(full_name_);
}

void CanvasProp::onFloat(t_float v)
{
    DataTypeProperty* prop = PropertyStorage::storage().acquire(full_name_);

    if (!prop) {
        OBJ_ERR << "property is not found: " << name_;
        return;
    }

    if (prop->isFloat()) {
        prop->setFloat(v);

        t_float f;
        if (prop->getFloat(f))
            floatTo(0, f);
    } else if (prop->isInt()) {
        prop->setInt(v);
        long i;
        if (prop->getInt(i))
            floatTo(0, i);
    } else if (prop->isBool()) {
        if (v != 0.0 && v != 1.0) {
            OBJ_ERR << "1 or 0 expected for bool property";
        } else {
            prop->setBool(v);

            bool b;
            if (prop->getBool(b))
                floatTo(0, b ? 1 : 0);
        }
    } else {
        OBJ_ERR << "unknown property type";
    }

    PropertyStorage::storage().release(full_name_);
}

void setup_canvas_prop()
{
    ObjectFactory<CanvasProp> obj("canvas.prop");
}
