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
#include "ceammc_convert.h"
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

const BaseObject* ceammc_to_base_object(t_object* x, bool check)
{
    if (check && !is_ceammc_base(x))
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

const UIObject* ceammc_to_ui_object(t_object* x, bool check)
{
    if (check && !is_ceammc_ui(x))
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
    static t_symbol* SYM_CANVAS = gensym("canvas");

    if (!x)
        return {};

    if (x->te_g.g_pd->c_name != SYM_CANVAS)
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

static MaybeFloat rangeValueFromCC(const PropertyInfo& info, t_float val)
{
    if (info.isFloat()) {
        switch (info.constraints()) {
        case PropValueConstraints::CLOSED_RANGE:
            return convert::lin2lin_clip<t_float, 0, 127>(val, info.minFloat(), info.maxFloat());
        case PropValueConstraints::OPEN_RANGE:
            return convert::lin2lin_clip<t_float, 1, 126>(val, info.minFloat(), info.maxFloat());
        case PropValueConstraints::OPEN_CLOSED_RANGE:
            return convert::lin2lin_clip<t_float, 1, 127>(val, info.minFloat(), info.maxFloat());
        case PropValueConstraints::CLOSED_OPEN_RANGE:
            return convert::lin2lin_clip<t_float, 0, 126>(val, info.minFloat(), info.maxFloat());
        default:
            return {};
        }
    } else if (info.isInt()) {
        switch (info.constraints()) {
        case PropValueConstraints::CLOSED_RANGE:
            return (int)convert::lin2lin_clip<t_float, 0, 127>(val, info.minInt(), info.maxInt());
        case PropValueConstraints::OPEN_RANGE:
            return (int)convert::lin2lin_clip<t_float, 1, 126>(val, info.minInt(), info.maxInt());
        case PropValueConstraints::OPEN_CLOSED_RANGE:
            return (int)convert::lin2lin_clip<t_float, 1, 127>(val, info.minInt(), info.maxInt());
        case PropValueConstraints::CLOSED_OPEN_RANGE:
            return (int)convert::lin2lin_clip<t_float, 0, 126>(val, info.minInt(), info.maxInt());
        default:
            return {};
        }
    } else if (info.isBool()) {
        return val > 64;
    } else
        return {};
}

PropertySetState ceammc_base_property_set_cc(t_object* x, t_symbol* key, t_float val, bool check)
{
    using PPS = PropertySetState;

    BaseObject* obj = const_cast<BaseObject*>(ceammc_to_base_object(x, check));
    if (!obj)
        return PPS::OTHER_TYPE;

    for (auto& p : obj->properties())
        LIB_ERR << p->name()->s_name;

    auto prop = obj->property(key);
    if (!prop)
        return PPS::ERROR_NOT_FOUND;

    auto& info = prop->infoT();
    // check rw
    if (!info.isReadWrite())
        return PPS::ERROR_ACCESS;

    if (!info.isNumeric())
        return PPS::ERROR_INVALID_TYPE;

    auto newval = rangeValueFromCC(info, val);
    if (!newval)
        return PPS::ERROR_NO_RANGE;

    Atom a(*newval);
    if (prop->set(AtomListView(a)))
        return PPS::OK;
    else
        return PPS::ERROR_SET_VALUE;
}

PropertySetState ceammc_ui_property_set_cc(t_object* x, t_symbol* key, t_float val, bool check)
{
    using PPS = PropertySetState;

    if (check && !is_ceammc_ui(x))
        return PPS::OTHER_TYPE;

    auto key0 = gensym(key->s_name + 1); // skip first @
    auto* c = reinterpret_cast<t_eclass*>(x->te_g.g_pd);
    auto objdsp = static_cast<UIDspObject*>(reinterpret_cast<t_edspbox*>(x));
    auto obj = static_cast<UIObject*>(reinterpret_cast<t_ebox*>(x));

    auto info = (c->c_dsp) ? objdsp->propertyInfo(key0) : obj->propertyInfo(key0);
    if (!info)
        return PPS::ERROR_NOT_FOUND;

    // check rw
    if (!info->isReadWrite())
        return PPS::ERROR_ACCESS;

    if (!info->isNumeric())
        return PPS::ERROR_INVALID_TYPE;

    auto newval = rangeValueFromCC(*info, val);
    if (!newval)
        return PPS::ERROR_NO_RANGE;

    Atom a(*newval);
    if (obj->setProperty(key0, AtomListView(a)))
        return PPS::OK;
    else
        return PPS::ERROR_SET_VALUE;
}

PropertySetState ceammc_faust_property_set_cc(t_object* x, t_symbol* key, t_float val, bool check)
{
    using PSS = PropertySetState;
    using FaustObj = PdObject<faust::FaustExternalBase>;

    if (check && !is_ceammc_faust(x))
        return PSS::OTHER_TYPE;

    auto* obj = reinterpret_cast<FaustObj*>(x)->impl;
    auto prop = obj->property(key);
    if (!prop)
        return PSS::ERROR_NOT_FOUND;

    auto& info = prop->infoT();
    // check rw
    if (!info.isReadWrite())
        return PSS::ERROR_ACCESS;

    if (!info.isNumeric())
        return PSS::ERROR_INVALID_TYPE;

    auto newval = rangeValueFromCC(info, val);
    if (!newval)
        return PSS::ERROR_NO_RANGE;

    Atom a(*newval);
    if (prop->set(AtomListView(a)))
        return PSS::OK;
    else
        return PSS::ERROR_SET_VALUE;
}

PropertySetState ceammc_abstraction_property_set_cc(t_object* x, t_symbol* key, t_float val, bool check)
{
    using PSS = PropertySetState;
    return PSS::OTHER_TYPE;
}

PropertySetState ceammc_property_set_cc(t_object* x, t_symbol* key, t_float val)
{
    LIB_ERR << "base prop";

    auto res = ceammc_base_property_set_cc(x, key, val, true);
    if (res == PropertySetState::OK || res != PropertySetState::OTHER_TYPE)
        return res;

    LIB_ERR << "ui prop";

    res = ceammc_ui_property_set_cc(x, key, val, true);
    if (res == PropertySetState::OK || res != PropertySetState::OTHER_TYPE)
        return res;

    LIB_ERR << "faust prop";

    res = ceammc_faust_property_set_cc(x, key, val, true);
    if (res == PropertySetState::OK || res != PropertySetState::OTHER_TYPE)
        return res;

    return PropertySetState::ERROR_NOT_FOUND;
}

const char* ceammc_property_set_cc_str_state(PropertySetState st)
{
    using PSS = PropertySetState;
    switch (st) {
    case PSS::OK:
        return "ok";
    case PSS::OTHER_TYPE:
        return "unknown object type";
    case PSS::ERROR_ACCESS:
        return "readonly property";
    case PSS::ERROR_INVALID_TYPE:
        return "non-numeric property type";
    case PSS::ERROR_NOT_FOUND:
        return "not found";
    case PSS::ERROR_NO_RANGE:
        return "not bounded property";
    case PSS::ERROR_SET_VALUE:
        return "set error";
    default:
        return "unknown error";
    }
}

}
