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
#include "ceammc_externals.h"
#include "ceammc_factory.h"
#include "ceammc_faust.h"
#include "ceammc_object.h"
#include "ceammc_object_info.h"
#include "ceammc_ui_object.h"
#include "datatype_property.h"

namespace ceammc {

void register_ui_external(t_class* c)
{
    ObjectInfoStorage::instance().addUI(c);
}

void register_flext_external(t_class* c)
{
    ObjectInfoStorage::instance().addFlext(c);
}

bool is_ceammc(t_object* x)
{
    if (is_ceammc_base(x)
        || is_ceammc_ui(x)
        || is_ceammc_flext(x)
        || is_ceammc_abstraction(x))
        return true;

    return false;
}

bool is_ceammc_base(t_object* x)
{
    if (!x)
        return false;

    return ObjectInfoStorage::instance().findInBase(x->te_g.g_pd);
}

bool is_ceammc_ui(t_object* x)
{
    if (!x)
        return false;

    return ObjectInfoStorage::instance().findInUI(x->te_g.g_pd);
}

bool is_ceammc_flext(t_object* x)
{
    if (!x)
        return false;

    return ObjectInfoStorage::instance().findInFlext(x->te_g.g_pd);
}

bool is_ceammc_abstraction(t_object* x)
{
    static t_symbol* SYM_PROP_DECL = gensym("prop.declare");
    static t_symbol* SYM_CANVAS = gensym("canvas");

    if (!x)
        return false;

    if (x->te_g.g_pd->c_name != SYM_CANVAS)
        return false;

    t_canvas* cnv = reinterpret_cast<t_canvas*>(x);
    for (t_gobj* y = cnv->gl_list; y; y = y->g_next) {
        if (y->g_pd->c_name == SYM_PROP_DECL)
            return true;
    }

    return false;
}

const BaseObject* ceammc_to_base_object(t_object* x)
{
    if (!is_ceammc_base(x))
        return nullptr;

    auto pd_obj = reinterpret_cast<PdObject<BaseObject>*>(x);
    return pd_obj->impl;
}

std::vector<PropertyInfo> ceammc_base_properties(t_object* x)
{
    const BaseObject* obj = ceammc_to_base_object(x);
    if (!obj) {
        std::cerr << "corrupted CEAMMC object: " << obj << "\n";
        return {};
    }

    std::vector<PropertyInfo> res;
    res.reserve(obj->getProperties().size());

    for (auto p : obj->getProperties()) {
        if (!p)
            continue;

        p->get();
        res.push_back(p->infoT());
    }

    return res;
}

const UIObject* ceammc_to_ui_object(t_object* x)
{
    if (!is_ceammc_ui(x))
        return nullptr;

    return reinterpret_cast<const UIObject*>(x);
}

std::vector<PropertyInfo> ceammc_ui_properties(t_object* x)
{
    if (!is_ceammc_ui(x))
        return {};

    auto* c = reinterpret_cast<t_eclass*>(x->te_g.g_pd);

    if (c->c_dsp)
        return static_cast<UIDspObject*>(reinterpret_cast<t_edspbox*>(x))->propsInfo();
    else
        return static_cast<UIObject*>(reinterpret_cast<t_ebox*>(x))->propsInfo();
}

std::vector<PropertyInfo> ceammc_abstraction_properties(t_object* x)
{
    static t_symbol* SYM_PROP_DECL = gensym("prop.declare");

    if (!is_ceammc_abstraction(x))
        return {};

    std::vector<PropertyInfo> res;
    t_canvas* cnv = reinterpret_cast<t_canvas*>(x);
    for (t_gobj* y = cnv->gl_list; y; y = y->g_next) {
        // skipping non [prop.declare]
        if (y->g_pd->c_name != SYM_PROP_DECL)
            continue;

        t_object* prop_declare = reinterpret_cast<t_object*>(y);
        int argc = binbuf_getnatom(prop_declare->te_binbuf);
        t_atom* argv = binbuf_getvec(prop_declare->te_binbuf);

        AtomList args(argc, argv);
        if (args.size() < 2 || !args[1].isSymbol()) {
            LIB_ERR << "empty args";
            continue;
        }

        std::string prop_name = "@";
        prop_name += args[1].asSymbol()->s_name;

        auto fn = PropertyStorage::makeFullName(prop_name, cnv);
        PropertyPtr pprop(fn);
        if (!pprop) {
            LIB_ERR << "can't find property: " << fn;
            continue;
        }

        res.push_back(pprop->info());
    }

    return res;
}

}
