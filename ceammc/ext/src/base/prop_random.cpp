/*****************************************************************************
 * Copyright 2021 Serge Poltavsky. All rights reserved.
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
#include "prop_random.h"
#include "ceammc_externals.h"
#include "ceammc_factory.h"
#include "datatype_property.h"

#include <chrono>

extern "C" {
#include "g_canvas.h"
#include "m_imp.h"
}

using URandomInt = std::uniform_int_distribution<int>;
using URandomFloat = std::uniform_real_distribution<t_float>;

PropRandom::PropRandom(const PdArgs& args)
    : BaseObject(args)
    , rnd_(std::time(0))
    , seed_(nullptr)
{
    seed_ = new IntProperty("@seed", 0);
    seed_->setSuccessFn([this](Property*) { rnd_.seed(seed_->value()); });
    addProperty(seed_);

    const auto NARGS = args.args.size();
    props_.reserve(NARGS);

    for (size_t i = 0; i < NARGS; i++) {
        const Atom& a = args.args[i];
        if (a.isProperty()) {
            props_.push_back(a.asSymbol());
            createInlet();
        } else if (i == 0)
            seed_->set(a);
        else
            OBJ_ERR << "property name expected (starting from '@'), got: " << a << ", skipping argument";
    }

    createOutlet();
    createOutlet();
}

void PropRandom::onBang()
{
    onInlet(1, {});
}

void PropRandom::onInlet(size_t n, const AtomListView&)
{
    // inlet index correction
    if (n-- == 0)
        return;

    t_outlet* outlet = nullptr;
    auto conn = obj_starttraverseoutlet(owner(), &outlet, 0);
    int connected = 0;

    while (conn) {
        t_object* dest = nullptr;
        t_inlet* inletp = nullptr;
        int whichp = 0;
        conn = obj_nexttraverseoutlet(conn, &dest, &inletp, &whichp);

        if (!dest)
            break;

        connected++;

        if (dest->te_g.g_pd == canvas_class)
            randomCanvasProps(reinterpret_cast<t_glist*>(dest), props_[n]);
        else
            randomObjectProps(dest, props_[n]);
    }

    if (!connected)
        OBJ_ERR << "not connected";
}

void PropRandom::randomCanvasProps(t_glist* dest, t_symbol* s)
{
    auto* full = PropertyStorage::makeFullName(s, dest);
    PropertyPtr pp(full);
    if (pp) {
        const auto has_range = pp->hasMinValue() && pp->hasMaxValue();

        if (pp->isBool()) {
            URandomInt dist(0, 1);
            auto val = dist(rnd_);
            pp->setBool(val);
            floatTo(1, val);
        } else if (pp->isInt() && has_range) {
            const auto range = pp->intRange();
            URandomInt dist(range.first, range.second);
            auto val = dist(rnd_);
            pp->setInt(val);
            floatTo(1, val);
        } else if (pp->isFloat() && has_range) {
            const auto range = pp->floatRange();
            URandomFloat dist(range.first, range.second);
            auto val = dist(rnd_);
            pp->setFloat(val);
            floatTo(1, val);
        } else if (pp->hasEnumValues()) {
            const auto& en = pp->enumValues();
            const auto ridx = URandomInt(0, en.size() - 1)(rnd_);
            pp->setFromPdArgs(en[ridx]);
            atomTo(1, en[ridx]);
        } else {
            OBJ_ERR << "can't randomize property: " << s->s_name;
        }
    } else
        OBJ_ERR << "can't find property: " << s->s_name;
}

void PropRandom::randomObjectProps(t_object* dest, t_symbol* s)
{
    if (!dest) {
        OBJ_ERR << "NULL object";
        return;
    }

    t_symbol* obj_name = dest->te_g.g_pd->c_name;
    auto fn = ceammc_get_propset_fn(dest);
    if (!fn) {
        OBJ_ERR << "can't find property " << s->s_name << " in object [" << obj_name->s_name << ']';
        return;
    }

    if (!is_ceammc(dest)) {
        OBJ_DBG << "can't get properties, not a CEAMMC object: [" << obj_name->s_name << "]";
        return;
    }

    std::vector<PropertyInfo> props;

    if (is_ceammc_base(dest))
        props = ceammc_base_properties(dest);
    else if (is_ceammc_ui(dest))
        props = ceammc_ui_properties(dest);
    else if (is_ceammc_abstraction(dest))
        props = ceammc_abstraction_properties(dest);
    else {
        OBJ_ERR << "not a CEAMMC object: [" << obj_name << ']';
        return;
    }

    bool prop_found = false;

    for (auto& pi : props) {
        if (pi.name() != s)
            continue;

        // prop found

        if (pi.isInitOnly()) {
            OBJ_ERR << "initonly property: " << s->s_name;
            break;
        }

        if (pi.isReadOnly()) {
            OBJ_ERR << "readonly property: " << s->s_name;
            break;
        }

        if (pi.isHidden()) {
            OBJ_ERR << "hidden property: " << s->s_name;
            break;
        }

        const auto has_range = pi.hasConstraintsMax() && pi.hasConstraintsMin();
        if (pi.isNumeric() && !has_range) {
            OBJ_ERR << "property " << s->s_name << " is not bounded";
            break;
        }

        Atom a;
        if (pi.isFloat()) {
            URandomFloat dist(pi.minFloat(), pi.maxFloat());
            a = dist(rnd_);
        } else if (pi.isInt()) {
            URandomInt dist(pi.minInt(), pi.maxInt());
            a = dist(rnd_);
        } else if (pi.isBool()) {
            URandomInt dist(0, 1);
            a = dist(rnd_);
        } else if (pi.enumValues().size() > 0) {
            const auto& enums = pi.enumValues();
            URandomInt dist(0, enums.size() - 1);
            a = enums[dist(rnd_)];
        } else {
            OBJ_ERR << "can't randomize property: " << s->s_name;
            break;
        }

        if (!fn(dest, s, 1, const_cast<t_atom*>(&a.atom()))) {
            OBJ_ERR << "can't set property " << s->s_name << " to " << a;
            break;
        } else
            atomTo(1, a);

        prop_found = true;
        break;
    }

    if (!prop_found)
        OBJ_ERR << "can't find property " << s->s_name << " in object [" << obj_name->s_name << ']';
}

const char* PropRandom::annotateInlet(size_t n) const
{
    if (n < props_.size())
        return props_[n]->s_name;
    else
        return nullptr;
}

void setup_prop_random()
{
    ObjectFactory<PropRandom> obj("prop.random", OBJECT_FACTORY_NO_DEFAULT_INLET);
    obj.parsePropsMode(PdArgs::PARSE_NONE);

    obj.setDescription("property randomizer");
}
