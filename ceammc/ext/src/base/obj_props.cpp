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
            dict->insert(Atom(p.name()), DataAtom(DataPtr(val)));

            val->insert("name", p.name());
            val->insert("type", typeToSymbol(p.type()));
            val->insert("view", viewToSymbol(p.view()));

            const Atom KEY_DEF(gensym("default"));

            if (p.isBool()) {
                bool b;
                if (p.getDefault(b))
                    val->insert(KEY_DEF, Atom(b ? 1 : 0));
            } else if (p.isFloat()) {
                t_float f;
                if (p.getDefault(f))
                    val->insert(KEY_DEF, f);
            } else if (p.isInt()) {
                int i;
                if (p.getDefault(i))
                    val->insert(KEY_DEF, Atom(i));
            } else if (p.isSymbol()) {
                t_symbol* s;
                if (p.getDefault(s))
                    val->insert(KEY_DEF, Atom(s));
            } else if (p.isVariant()) {
                Atom a;
                if (p.getDefault(a))
                    val->insert(KEY_DEF, a);
            } else if (p.isList()) {
                AtomList l;
                if (p.getDefault(l))
                    val->insert(KEY_DEF, l);
            }

            if (p.hasConstraintsMin()) {
                if (p.isFloat())
                    val->insert("min", p.minFloat());
                else if (p.isInt())
                    val->insert("min", p.minInt());
            }

            if (p.hasConstraintsMax()) {
                if (p.isFloat())
                    val->insert("max", p.maxFloat());
                else if (p.isInt())
                    val->insert("max", p.maxInt());
            }

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
