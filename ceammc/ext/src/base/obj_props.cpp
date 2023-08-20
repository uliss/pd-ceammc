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
#include "obj_props.h"
#include "ceammc_externals.h"
#include "ceammc_factory.h"
#include "datatype_dict.h"

extern "C" {
#include "m_imp.h"
}

static t_symbol* typeToSymbol(PropValueType t)
{
    static t_symbol* TYPE_NAMES[] = {
        gensym("bool"), gensym("int"), gensym("float"),
        gensym("symbol"), gensym("variant"), gensym("list")
    };

    static_assert(sizeof(TYPE_NAMES) / sizeof(TYPE_NAMES[0]) == int(PropValueType::LIST) + 1,
        "type to symbol table size mismatch");

    return TYPE_NAMES[int(t)];
}

static t_symbol* viewToSymbol(PropValueView v)
{
    static t_symbol* VIEW_NAMES[] = {
        gensym("slider"), gensym("knob"), gensym("numbox"),
        gensym("spinbox"), gensym("toggle"), gensym("menu"),
        gensym("entry"), gensym("color")
    };

    static_assert(sizeof(VIEW_NAMES) / sizeof(VIEW_NAMES[0]) == int(PropValueView::COLOR) + 1,
        "view to symbol table size mismatch");

    return VIEW_NAMES[int(v)];
}

ObjProps::ObjProps(const PdArgs& args)
    : NodeExternal(args)
{
    createOutlet();
    createOutlet();
}

void ObjProps::onBang()
{
    ObjectList lst = connectedObjects();

    if (lst.size() > 1)
        OBJ_DBG << "warning: several objects connected";

    auto KEY_DEF = gensym("default");

    AtomList obj_list;

    for (t_object* o : lst) {
        t_symbol* obj_name = o->te_g.g_pd->c_name;

        if (!is_ceammc(o)) {
            OBJ_DBG << "can't get properties, not a CEAMMC object: [" << obj_name->s_name << "]";
            continue;
        }

        std::vector<PropertyInfo> props;

        if (is_ceammc_base(o))
            props = ceammc_base_properties(o);
        if (is_ceammc_ui(o))
            props = ceammc_ui_properties(o);
        if (is_ceammc_abstraction(o))
            props = ceammc_abstraction_properties(o);

        DictAtom obj_info;
        obj_info->insert("name", obj_name);
        obj_info->insert("properties", AtomList());
        auto& obj_props = obj_info->at("properties");

        for (PropertyInfo& p : props) {
            DictAtom prop_info;

            obj_props.append(prop_info);

            prop_info->insert("name", p.name());
            prop_info->insert("type", typeToSymbol(p.type()));
            prop_info->insert("view", viewToSymbol(p.view()));

            if (p.isBool()) {
                bool b;
                if (p.getDefault(b))
                    prop_info->insert(KEY_DEF, Atom(b ? 1 : 0));
            } else if (p.isFloat()) {
                t_float f;
                if (p.getDefault(f))
                    prop_info->insert(KEY_DEF, f);
            } else if (p.isInt()) {
                int i;
                if (p.getDefault(i))
                    prop_info->insert(KEY_DEF, Atom(i));
            } else if (p.isSymbol()) {
                t_symbol* s;
                if (p.getDefault(s))
                    prop_info->insert(KEY_DEF, Atom(s));
            } else if (p.isVariant()) {
                Atom a;
                if (p.getDefault(a))
                    prop_info->insert(KEY_DEF, a);
            } else if (p.isList()) {
                AtomList l;
                if (p.getDefault(l))
                    prop_info->insert(KEY_DEF, l);
            }

            if (p.hasConstraintsMin()) {
                if (p.isFloat())
                    prop_info->insert("min", p.minFloat());
                else if (p.isInt())
                    prop_info->insert("min", p.minInt());
            }

            if (p.hasConstraintsMax()) {
                if (p.isFloat())
                    prop_info->insert("max", p.maxFloat());
                else if (p.isInt())
                    prop_info->insert("max", p.maxInt());
            }

            if (p.hasStep())
                prop_info->insert("step", p.step());

            if (p.hasEnumLimit())
                prop_info->insert("enum", p.enumValues());

            if (!p.equalUnit(PropValueUnits::NONE)) {
                AtomList units;
                p.unitsIterate([&units](const char* u) {
                    units.append(gensym(u));
                });
                prop_info->insert("units", units);
            }
        }

        obj_list.append(obj_info);
    }

    listTo(0, obj_list);
}

void setup_obj_props()
{
    ObjectFactory<ObjProps> obj("obj.props");

    obj.setDescription("property extractor");
    obj.addAuthor("Serge Poltavsky");
    obj.setKeywords({ "property" });
    obj.setCategory("base");
    obj.setSinceVersion(0, 7);
}
