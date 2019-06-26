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
#include "prop_declare.h"
#include "../string/tinyformat.h"
#include "../string/vformatlist.h"
#include "ceammc_canvas.h"
#include "ceammc_convert.h"
#include "ceammc_factory.h"
#include "ceammc_globaldata.h"
#include "datatype_property.h"

#include <cstring>
#include <limits>
#include <unordered_map>

extern "C" {
#include "g_canvas.h"
#include "m_imp.h"
}

static t_symbol* SYM_BOOL;
static t_symbol* SYM_INT;
static t_symbol* SYM_ENUM;

static const t_float FMIN = std::numeric_limits<t_float>::lowest();
static const t_float FMAX = std::numeric_limits<t_float>::max();
static const long IMIN = std::numeric_limits<long>::min();
static const long IMAX = std::numeric_limits<long>::max();

class CanvasEditCallback {
    std::unordered_map<t_canvas*, t_glistkeyfn> map_;
    CanvasEditCallback() {}

public:
    static CanvasEditCallback& instance()
    {
        static CanvasEditCallback c;
        return c;
    }

    bool contains(t_canvas* c) const
    {
        return map_.find(c) == map_.end();
    }

    void insert(t_canvas* c, t_glistkeyfn fn)
    {
        map_.insert({ c, fn });
    }

    void remove(t_canvas* c)
    {
        map_.erase(c);
    }

    t_glistkeyfn get(t_canvas* c)
    {
        return map_.at(c);
    }
};

static void prop_declare_loadbang(t_object* x, t_floatarg action)
{
    if (action == LB_LOAD) {
        PropDeclare* prop = reinterpret_cast<PdObject<PropDeclare>*>(x)->impl;
        prop->onLoadBang();
    }
}

PropDeclare::PropDeclare(const PdArgs& args)
    : BaseObject(args)
    , sym_name_(&s_)
    , sym_full_name_(&s_)
    , type_(nullptr)
    , min_(nullptr)
    , max_(nullptr)
    , enum_(nullptr)
    , pprop_(nullptr)
{
    initName();

    if (PropertyStorage::storage().contains(full_name_)) {
        auto msg = tfm::format("\"%s\" is already declared", sym_name_->s_name);
        throw std::runtime_error(msg);
    }

    auto pprop = new DataTypeProperty(gensym(full_name_.c_str()));
    if (!PropertyStorage::storage().create(full_name_, pprop)) {
        delete pprop;
        throw std::runtime_error("can't create property");
    }

    pprop_ = PropertyStorage::storage().acquire(full_name_);

    type_ = new SymbolEnumProperty("@type", &s_float);
    type_->appendEnum(SYM_BOOL);
    type_->appendEnum(SYM_INT);
    type_->appendEnum(SYM_ENUM);
    type_->appendEnum(&s_symbol);
    type_->appendEnum(&s_list);
    createProperty(type_);

    createProperty(new SymbolEnumAlias("@f", type_, &s_float));
    createProperty(new SymbolEnumAlias("@s", type_, &s_symbol));
    createProperty(new SymbolEnumAlias("@l", type_, &s_list));
    createProperty(new SymbolEnumAlias("@i", type_, SYM_INT));
    createProperty(new SymbolEnumAlias("@b", type_, SYM_BOOL));

    min_ = new FloatProperty("@min", FMIN);
    max_ = new FloatProperty("@max", FMAX);
    enum_ = new ListProperty("@enum", AtomList());
    default_ = new ListProperty("@default", AtomList());

    createProperty(min_);
    createProperty(max_);
    createProperty(enum_);
    createProperty(default_);
}

PropDeclare::~PropDeclare()
{
    PropertyStorage::storage().release(full_name_);
}

void PropDeclare::parseProperties()
{
    BaseObject::parseProperties();

    max_->setReadonly(true);
    min_->setReadonly(true);
    enum_->setReadonly(true);
    default_->setReadonly(true);

    if (isFloat()) {
        pprop_->setTypeFloat(atomlistToValue<t_float>(default_->value(), 0.f));

        if (!pprop_->setFloatRange(min_->value(), max_->value())) {
            OBJ_ERR << "can't set range";
            return;
        }
    } else if (isInt()) {
        pprop_->setTypeInt(atomlistToValue<int>(default_->value(), 0));

        auto fmin = min_->value();
        auto fmax = max_->value();

        long lmin = 0;
        if (fmin == FMIN)
            lmin = IMIN;
        else
            lmin = fmin;

        long lmax = 0;
        if (fmax == FMAX)
            lmax = IMAX;
        else
            lmax = fmax;

        if (!pprop_->setIntRange(lmin, lmax)) {
            OBJ_ERR << "can't set range";
            return;
        }
    } else if (isBool()) {
        pprop_->setTypeBool(atomlistToValue<int>(default_->value(), false));
    } else if (isList()) {
        pprop_->setTypeList(default_->value());
    } else if (isSymbol()) {
        pprop_->setTypeSymbol(atomlistToValue<t_symbol*>(default_->value(), &s_));
        pprop_->setEnumValues(enum_->get());
    }
}

t_symbol* PropDeclare::name() const
{
    return sym_name_;
}

t_symbol* PropDeclare::fullName() const
{
    return sym_full_name_;
}

void PropDeclare::onLoadBang()
{
    t_canvas* cnv = canvas();
    if (!cnv)
        return;

    AtomList pv;
    // no property defined in canvas arguments
    if (!canvas_info_args(cnv).property(sym_name_->s_name, &pv)) {
        // output default values
        if (sym_full_name_->s_thing)
            pd_bang(sym_full_name_->s_thing);

        return;
    }

    PropertyPtr pptr(full_name_);
    if (!pptr)
        return;

    if (!pptr->setFromPdArgs(pv))
        OBJ_ERR << "error setting property: " << sym_name_;

    if (sym_full_name_->s_thing)
        pd_bang(sym_full_name_->s_thing);
}

bool PropDeclare::isFloat() const
{
    return type_->value() == &s_float;
}

bool PropDeclare::isInt() const
{
    return type_->value() == SYM_INT;
}

bool PropDeclare::isBool() const
{
    return type_->value() == SYM_BOOL;
}

bool PropDeclare::isSymbol() const
{
    return type_->value() == &s_symbol;
}

bool PropDeclare::isList() const
{
    return type_->value() == &s_list;
}

void PropDeclare::initName()
{
    if (args().empty() || !args()[0].isSymbol())
        sym_name_ = gensym("@default");
    else {
        char buf[64] = "@";
        strncat(buf, args()[0].asSymbol()->s_name, sizeof(buf) - 2);
        sym_name_ = gensym(buf);
    }

    full_name_ = PropertyStorage::makeFullName(sym_name_->s_name, canvas());
    sym_full_name_ = gensym(full_name_.c_str());
}

void setup_prop_declare()
{
    SYM_BOOL = gensym("bool");
    SYM_INT = gensym("int");
    SYM_ENUM = gensym("enum");

    ObjectFactory<PropDeclare> obj("prop.declare", OBJECT_FACTORY_NO_DEFAULT_INLET);
    class_addmethod(obj.classPointer(), (t_method)prop_declare_loadbang, gensym("loadbang"), A_DEFFLOAT, 0);
}
