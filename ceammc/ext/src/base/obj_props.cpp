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

static t_symbol* typeToSymbol(PropertyInfoType t)
{
    static t_symbol* TYPE_NAMES[] = {
        gensym("bool"), gensym("int"), gensym("float"),
        gensym("symbol"), gensym("variant"), gensym("list")
    };

    static_assert(sizeof(TYPE_NAMES) / sizeof(TYPE_NAMES[0]) == int(PropertyInfoType::LIST) + 1,
        "type to symbol table size mismatch");

    return TYPE_NAMES[int(t)];
}

static t_symbol* viewToSymbol(PropertyInfoView v)
{
    static t_symbol* VIEW_NAMES[] = {
        gensym("slider"), gensym("knob"), gensym("numbox"),
        gensym("spinbox"), gensym("toggle"), gensym("menu"),
        gensym("entry"), gensym("color")
    };

    static_assert(sizeof(VIEW_NAMES) / sizeof(VIEW_NAMES[0]) == int(PropertyInfoView::COLOR) + 1,
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

    DataTypeDict* dict = new DataTypeDict;
    DataPtr pdict(dict);

    for (t_object* o : lst) {
        if (!is_ceammc(o)) {
            OBJ_DBG << "not a CEAMMC object: " << o->te_g.g_pd->c_name->s_name;
            continue;
        }

        std::vector<PropertyInfo> props;

        if (is_ceammc_base(o))
            props = ceammc_base_properties(o);
        if (is_ceammc_ui(o))
            props = ceammc_ui_properties(o);
        if (is_ceammc_faust(o))
            props = ceammc_faust_properties(o);
        if (is_ceammc_abstraction(o))
            props = ceammc_abstraction_properties(o);

        for (PropertyInfo& p : props) {

            DataTypeDict* val = new DataTypeDict;
            dict->insert(Atom(gensym(p.name().c_str())), DataAtom(DataPtr(val)));

            val->insert("name", p.name());
            val->insert("type", typeToSymbol(p.type()));
            val->insert("view", viewToSymbol(p.view()));

            PropertyValue defval = p.defaultValue();
            if (defval.type() == typeid(AtomList))
                val->insert("default", boost::get<AtomList>(defval));
            else if (defval.type() == typeid(PropertySingleValue)) {
                PropertySingleValue sv = boost::get<PropertySingleValue>(defval);
                Atom def;

                if (sv.type() == typeid(bool))
                    def.setFloat(boost::get<bool>(sv) ? 1 : 0, true);
                else if (sv.type() == typeid(int))
                    def.setFloat(boost::get<int>(sv), true);
                else if (sv.type() == typeid(float))
                    def.setFloat(boost::get<float>(sv), true);
                else if (sv.type() == typeid(t_symbol*))
                    def.setSymbol(boost::get<t_symbol*>(sv), true);
                else if (sv.type() == typeid(Atom))
                    def = boost::get<Atom>(sv);

                if (!def.isNone())
                    val->insert("default", def);
            }

            if (p.hasMinLimit())
                val->insert("min", p.min());

            if (p.hasMaxLimit())
                val->insert("max", p.max());

            if (p.hasStep())
                val->insert("step", p.step());

            if (p.hasEnumLimit())
                val->insert("enum", p.enumValues());
        }
    }

    dataTo(0, pdict);
}

void setup_obj_props()
{
    ObjectFactory<ObjProps> obj("obj.props");
}
