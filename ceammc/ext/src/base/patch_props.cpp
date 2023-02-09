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
#include "patch_props.h"
#include "ceammc_canvas.h"
#include "ceammc_factory.h"
#include "datatype_property.h"
#include "prop_declare.h"

#include <unordered_map>

extern "C" {
#include "g_canvas.h"
#include "m_imp.h"
}

PatchProps::PatchProps(const PdArgs& args)
    : BaseObject(args)
{
    createOutlet();
}

bool PatchProps::processAnyProps(t_symbol* sel, const AtomListView& lst)
{
    return false;
}

void PatchProps::onBang()
{
    t_canvas* cnv = canvas();
    if (!cnv)
        return;

    for (t_gobj* x = cnv->gl_list; x != nullptr; x = x->g_next) {
        if (x->g_pd != ObjectFactory<PropDeclare>::classPointer())
            continue;

        PropDeclare* prop = ObjectFactory<PropDeclare>::fromObject((t_object*)x);
        t_symbol* prop_name = prop->fullName();
        if (prop_name->s_thing)
            pd_bang(prop_name->s_thing);
    }
}

void PatchProps::onAny(t_symbol* s, const AtomListView& l)
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
        PropertyPtr pprop(full_name);

        if (!pprop) {
            OBJ_ERR << "property is not found: " << name;
            return;
        }

        if (pprop->isFloat()) {
            t_float f;
            if (pprop->getFloat(f))
                outputProp(name, f);
        } else if (pprop->isInt()) {
            int i;
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

    } else {
        auto full_name = PropertyStorage::makeFullName(sel, canvas());
        PropertyPtr pprop(full_name);

        if (!pprop) {
            OBJ_ERR << "property is not found: " << sel;
            return;
        }

        pprop->setFromPdArgs(l);
    }
}

void PatchProps::m_all_props(t_symbol* s, const AtomListView& args)
{
    t_canvas* x = canvas();
    if (!x)
        return;

    AtomList res;

    for (t_gobj* y = x->gl_list; y; y = y->g_next) {
        if (y->g_pd != ObjectFactory<PropDeclare>::classPointer())
            continue;

        PropDeclare* prop = ObjectFactory<PropDeclare>::fromObject((t_object*)y);
        res.append(prop->name());
    }

    anyTo(0, SYM_PROPS_ALL(), res);
}

void PatchProps::m_default(t_symbol*, const AtomListView&)
{
    t_canvas* cnv = canvas();
    if (!cnv)
        return;

    for (t_gobj* x = cnv->gl_list; x; x = x->g_next) {
        if (x->g_pd != ObjectFactory<PropDeclare>::classPointer())
            continue;

        PdObject<PropDeclare>* prop = reinterpret_cast<PdObject<PropDeclare>*>(x);
        t_symbol* sym = prop->impl->fullName();
        if (sym->s_thing)
            pd_typedmess(sym->s_thing, gensym("default"), 0, 0);
    }
}

void PatchProps::dump() const
{
    BaseObject::dump();

    t_canvas* cnv = (t_canvas*)canvas();
    if (!cnv)
        return;

    for (t_gobj* x = cnv->gl_list; x; x = x->g_next) {
        if (x->g_pd != ObjectFactory<PropDeclare>::classPointer())
            continue;

        PropertyPtr pprop(ObjectFactory<PropDeclare>::fromObject((t_object*)x)->fullName());
        if (pprop) {
            OBJ_DBG << "full name:   " << pprop->name()->s_name << "\n"
                    << "type:        " << to_string(pprop->propertyType()) << "\n"
                    << "value:       " << pprop->propertyStrValue();

            if (pprop->hasMinValue())
                OBJ_DBG << "min:       " << pprop->propertyStrMinValue();
            if (pprop->hasMaxValue())
                OBJ_DBG << "max:       " << pprop->propertyStrMaxValue();
            if (pprop->hasEnumValues())
                OBJ_DBG << "enum:      " << pprop->enumValues();
        }
    }
}

void PatchProps::outputProp(const std::string& name, t_float f)
{
    anyTo(0, gensym(name.c_str()), Atom(f));
}

void PatchProps::outputProp(const std::string& name, t_symbol* s)
{
    anyTo(0, gensym(name.c_str()), Atom(s));
}

void PatchProps::outputProp(const std::string& name, const AtomList& l)
{
    anyTo(0, gensym(name.c_str()), l);
}

void setup_patch_props()
{
    ObjectFactory<PatchProps> obj("patch.props");
    obj.addMethod("@*?", &PatchProps::m_all_props);
    obj.addMethod("default", &PatchProps::m_default);

    obj.setDescription("patch properties manager");
}
