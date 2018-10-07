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
#include "canvas_props.h"
#include "ceammc_canvas.h"
#include "ceammc_factory.h"
#include "datatype_property.h"
#include "prop_declare.h"

#include <unordered_map>

extern "C" {
#include "g_canvas.h"
#include "m_imp.h"
}

static void props_loadbang(t_object* x, t_floatarg action)
{
    if (action == LB_LOAD) {
        CanvasProps* props = reinterpret_cast<PdObject<CanvasProps>*>(x)->impl;
        props->onLoadBang();
    }
}

CanvasProps::CanvasProps(const PdArgs& args)
    : BaseObject(args)
{
    createOutlet();
}

bool CanvasProps::processAnyProps(t_symbol* sel, const AtomList& lst)
{
    return false;
}

void CanvasProps::onBang()
{
    t_canvas* x = canvas();
    if (!x)
        return;

    for (t_gobj* y = x->gl_list; y; y = y->g_next) {
        if (y->g_pd->c_name != PropDeclare::className)
            continue;

        PdObject<PropDeclare>* prop = reinterpret_cast<PdObject<PropDeclare>*>(y);
        t_symbol* prop_name = prop->impl->fullName();
        if (prop_name->s_thing)
            pd_bang(prop_name->s_thing);
    }
}

void CanvasProps::onAny(t_symbol* s, const AtomList& l)
{
    if (s->s_name[0] != '@')
        return;

    std::string sel(s->s_name);
    if (sel.size() < 2)
        return;

    // prop request
    if (sel.back() == '?') {
        std::string name = sel.substr(0, sel.size() - 1);
        auto full_name = PropertyStorage::makeFullName(name, canvas());
        DataTypeProperty* pprop = PropertyStorage::storage().acquire(full_name);
        if (!pprop) {
            OBJ_ERR << "property is not found: " << name;
            return;
        }

        if (pprop->isFloat()) {
            t_float f;
            if (pprop->getFloat(f))
                outputProp(name, f);
        } else if (pprop->isInt()) {
            long i;
            if (pprop->getInt(i))
                outputProp(name, i);
        } else if (pprop->isBool()) {
            bool b;
            if (pprop->getBool(b))
                outputProp(name, b ? 1 : 0);
        } else if (pprop->isSymbol()) {
            t_symbol* s;
            if (pprop->getSymbol(&s))
                outputProp(name, s);
        } else if (pprop->isList()) {
            AtomList l;
            if (pprop->getList(l))
                outputProp(name, l);
        } else {
            OBJ_ERR << "unhandled property type: " << name;
        }

        PropertyStorage::storage().release(full_name);
    } else {
        auto full_name = PropertyStorage::makeFullName(sel, canvas());

        DataTypeProperty* pprop = PropertyStorage::storage().acquire(full_name);
        if (!pprop) {
            OBJ_ERR << "property is not found: " << sel;
            return;
        }

        if (pprop->isFloat()) {
            if (l.isFloat())
                pprop->setFloat(l[0].asFloat());
            else
                OBJ_ERR << sel << " float argument is expected: " << l;
        } else if (pprop->isInt()) {
            if (l.isFloat())
                pprop->setInt(l[0].asFloat());
            else
                OBJ_ERR << sel << "int argument is expected: " << l;
        } else if (pprop->isBool()) {
            if (l.isFloat() && (l[0].asFloat() == 0.0 || l[0].asFloat() == 1.0)) {
                pprop->setBool(l[0].asFloat());
            } else
                OBJ_ERR << sel << "1 or 0 is expected: " << l;
        } else if (pprop->isSymbol()) {
            if (l.isSymbol()) {
                pprop->setSymbol(l[0].asSymbol());
            } else
                OBJ_ERR << sel << "symbol is expected: " << l;
        } else if (pprop->isList()) {
            pprop->setList(l);
        } else {
            OBJ_ERR << "unhandled property type: " << sel;
        }

        PropertyStorage::storage().release(full_name);
    }
}

void CanvasProps::onLoadBang()
{
    t_canvas* cnv = canvas();
    if (!cnv)
        return;

    typedef std::unordered_map<t_symbol*, AtomList> PropMap;
    PropMap props_map;
    auto cnv_args = canvas_info_args(cnv);
    PropMap::iterator last_prop = props_map.end();
    int nprops = 0;
    for (Atom& a : cnv_args) {
        if (a.isProperty()) {
            nprops++;
            last_prop = props_map.insert(std::make_pair(a.asSymbol(), AtomList())).first;
        } else {
            if (nprops == 0)
                continue;

            last_prop->second.append(a);
        }
    }

    for (t_gobj* x = cnv->gl_list; x; x = x->g_next) {
        if (x->g_pd->c_name != PropDeclare::className)
            continue;

        PropDeclare* prop = reinterpret_cast<PdObject<PropDeclare>*>(x)->impl;
        t_symbol* prop_name = prop->name();
        auto it = props_map.find(prop_name);
        // using default property value
        if (it == props_map.end()) {
            auto full_name = prop->fullName();
            if (full_name->s_thing)
                pd_bang(full_name->s_thing);

            continue;
        } else
            onAny(prop_name, it->second);
    }
}

void CanvasProps::m_all_props(t_symbol* s, const AtomList& args)
{
    t_canvas* x = canvas();
    if (!x)
        return;

    AtomList res;
    char buf[MAXPDSTRING];

    for (t_gobj* y = x->gl_list; y; y = y->g_next) {
        if (y->g_pd->c_name != PropDeclare::className)
            continue;

        PropDeclare* prop = reinterpret_cast<PdObject<PropDeclare>*>(y)->impl;
        res.append(prop->name());
    }

    anyTo(0, SYM_PROPS_ALL(), res);
}

void CanvasProps::m_default(t_symbol*, const AtomList&)
{
    t_canvas* cnv = canvas();
    if (!cnv)
        return;

    for (t_gobj* x = cnv->gl_list; x; x = x->g_next) {
        if (x->g_pd->c_name != PropDeclare::className)
            continue;

        PdObject<PropDeclare>* prop = reinterpret_cast<PdObject<PropDeclare>*>(x);
        t_symbol* sym = prop->impl->fullName();
        if (sym->s_thing)
            pd_typedmess(sym->s_thing, gensym("default"), 0, 0);
    }
}

void CanvasProps::dump() const
{
    BaseObject::dump();

    t_canvas* cnv = (t_canvas*) canvas();
    if (!cnv)
        return;

    for (t_gobj* x = cnv->gl_list; x; x = x->g_next) {
        if (x->g_pd->c_name != PropDeclare::className)
            continue;

        PropDeclare* prop = reinterpret_cast<PdObject<PropDeclare>*>(x)->impl;
        t_symbol* prop_name = prop->name();
        OBJ_DBG << prop_name->s_name;
    }
}

void CanvasProps::outputProp(const std::string& name, t_float f)
{
    anyTo(0, gensym(name.c_str()), Atom(f));
}

void CanvasProps::outputProp(const std::string& name, t_symbol* s)
{
    anyTo(0, gensym(name.c_str()), Atom(s));
}

void CanvasProps::outputProp(const std::string& name, const AtomList& l)
{
    anyTo(0, gensym(name.c_str()), l);
}

void setup_canvas_props()
{
    ObjectFactory<CanvasProps> obj("canvas.props");
    obj.addMethod("@*?", &CanvasProps::m_all_props);
    obj.addMethod("default", &CanvasProps::m_default);
    class_addmethod(obj.classPointer(), (t_method)props_loadbang, gensym("loadbang"), A_DEFFLOAT, 0);
}
