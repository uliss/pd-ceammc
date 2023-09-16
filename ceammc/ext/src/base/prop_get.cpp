/*****************************************************************************
 * Copyright 2020 Serge Poltavsky. All rights reserved.
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
#include "prop_get.h"
#include "ceammc_factory.h"
#include "ceammc_output.h"
#include "datatype_property.h"
#include "fmt/format.h"

extern "C" {
#include "g_canvas.h"
#include "m_imp.h"
}

PropGet::PropGet(const PdArgs& args)
    : BaseObject(args)
{
    createOutlet();

    for (auto& a : args.args) {
        if (a.isProperty()) {
            props_.push_back(a.asSymbol());
            createOutlet();
        } else
            OBJ_ERR << "property name expected (starting from '@'), got: " << a << ", skipping argument";
    }

    if (numOutlets() < 2)
        createOutlet();
}

t_object* PropGet::findDestination()
{
    t_outlet* outlet = nullptr;
    auto conn = obj_starttraverseoutlet(owner(), &outlet, 0);
    if (!conn) {
        OBJ_ERR << "not connected to object";
        return nullptr;
    }

    t_object* dest;
    t_inlet* inletp;
    int whichp;

    if (obj_nexttraverseoutlet(conn, &dest, &inletp, &whichp))
        OBJ_ERR << "warning: connected to several objects, will get property only from the first one";

    if (!dest) {
        OBJ_ERR << "invalid connection";
        return nullptr;
    } else
        return dest;
}

void PropGet::outputProperties(t_object* dest, const std::vector<t_symbol*>& props)
{
    if (!dest)
        return;

    if (dest->te_g.g_pd == canvas_class)
        processAbstractionProps(reinterpret_cast<t_canvas*>(dest), props);
    else
        processObjectProps(dest, props);
}

void PropGet::onBang()
{
    outputProperties(findDestination(), props_);
}

void PropGet::onClick(t_floatarg xpos, t_floatarg ypos, t_floatarg shift, t_floatarg ctrl, t_floatarg alt)
{
    onBang();
}

const char* PropGet::annotateOutlet(size_t n) const
{
    if (n == 0)
        return "connect to destination object";
    else if (n <= props_.size())
        return props_[n - 1]->s_name;
    else
        return nullptr;
}

void PropGet::processObjectProps(t_object* dest, const std::vector<t_symbol*>& props)
{
    auto fn = ceammc_get_propget_fn(dest);
    if (!fn) {
        OBJ_ERR << fmt::format("object has no properties: [{}]", class_getname(dest->te_g.g_pd));
        return;
    }

    for (auto it = props.rbegin(); it != props.rend(); ++it) {
        int argc = 0;
        t_atom* argv = nullptr;
        int rc = fn(dest, *it, &argc, &argv);
        if (!rc) {
            OBJ_ERR << fmt::format("property '{}' not found in object [{}]", (*it)->s_name, class_getname(dest->te_g.g_pd));
            continue;
        }

        const size_t IDX = props.size() - std::distance(props.rbegin(), it);
        outletAtomList(outletAt(IDX), AtomListView(argv, argc), true);

        freebytes(argv, argc);
    }
}

void PropGet::processAbstractionProps(t_glist* dest, const std::vector<t_symbol*>& props)
{
    for (size_t i = 0; i < props.size(); i++) {
        auto* full = PropertyStorage::makeFullName(props[i], dest);
        PropertyPtr pp(full);
        const size_t IDX = i + 1;
        if (pp) {
            if (pp->isBool()) {
                bool b = false;
                pp->getBool(b);
                boolTo(IDX, b);
            } else if (pp->isInt()) {
                int ii = 0;
                pp->getInt(ii);
                floatTo(IDX, ii);
            } else if (pp->isFloat()) {
                t_float f = 0;
                pp->getFloat(f);
                floatTo(IDX, f);
            } else if (pp->isSymbol()) {
                t_symbol* s = &s_;
                pp->getSymbol(&s);
                symbolTo(IDX, s);
            } else if (pp->isList()) {
                AtomList l;
                pp->getList(l);
                listTo(IDX, l);
            } else {
                OBJ_ERR << "unknown property type: " << (int)pp->propertyType();
            }
        } else {
            OBJ_ERR << "property not found: " << props[i]->s_name;
        }
    }
}

void PropGet::onAny(t_symbol* s, const AtomListView& /*lv*/)
{
    if (!Atom(s).isProperty()) {
        OBJ_ERR << "property name expected, got: " << s;
        return;
    }

    const auto len = strlen(s->s_name);
    if (s->s_name[len - 1] == '?') {
        // remove trailing '?'
        char buf[len];
        memcpy(buf, s->s_name, len);
        buf[len - 1] = '\0';
        outputProperties(findDestination(), { gensym(buf) });
    } else
        outputProperties(findDestination(), { s });
}

void setup_prop_get()
{
    ObjectFactory<PropGet> obj("prop.get");
    obj.addAlias("p.get");
    obj.addInletInfo("bang:  get specified properties\n"
                     "click: same as bang");
    obj.useClick();
    obj.noPropsDispatch();
    obj.noArgsAndPropsParse();

    obj.setDescription("get property from connected object/subpatch/abstraction");
    obj.setCategory("property");
    obj.setKeywords({"property"});
}
