/*****************************************************************************
 * Copyright 2016 Serge Poltavsky. All rights reserved.
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

#include "ceammc_object.h"

namespace ceammc {

t_outlet* BaseObject::outletAt(size_t n)
{
    if (n >= outlets_.size()) {
        post("[%s]: ERROR! invalid outlet index: %Ul", name_.c_str(), n);
        return 0;
    }

    return outlets_[n];
}

void BaseObject::createProperty(const char* name)
{
    props_[gensym(name)] = AtomList();
}

void BaseObject::bangTo(size_t n)
{
    if (n >= outlets_.size()) {
        post("[%s]: ERROR! invalid outlet index: %Ul", name_.c_str(), n);
        return;
    }
    outlet_bang(outlets_[n]);
}

void BaseObject::floatTo(size_t n, float v)
{
    if (n >= outlets_.size()) {
        post("[%s]: ERROR! invalid outlet index: %Ul", name_.c_str(), n);
        return;
    }
    outlet_float(outlets_[n], v);
}

void BaseObject::symbolTo(size_t n, t_symbol* s)
{
    if (n >= outlets_.size()) {
        post("[%s]: ERROR! invalid outlet index: %Ul", name_.c_str(), n);
        return;
    }
    outlet_symbol(outlets_[n], s);
}

void BaseObject::atomTo(size_t n, const Atom& a)
{
    if (n >= outlets_.size()) {
        post("[%s]: ERROR! invalid outlet index: %Ul", name_.c_str(), n);
        return;
    }

    a.output(outlets_[n]);
}

void BaseObject::listTo(size_t n, const AtomList& l)
{
    if (n >= outlets_.size()) {
        post("[%s]: ERROR! invalid outlet index: %Ul", name_.c_str(), n);
        return;
    }

    l.output(outlets_[n]);
}

void BaseObject::anyTo(size_t n, t_symbol* s, const Atom& a)
{
    if (n >= outlets_.size()) {
        post("[%s]: ERROR! invalid outlet index: %Ul", name_.c_str(), n);
        return;
    }

    a.outputAsAny(outlets_[n], s);
}

void BaseObject::anyTo(size_t n, t_symbol* s, const AtomList& l)
{
    if (n >= outlets_.size()) {
        post("[%s]: ERROR! invalid outlet index: %Ul", name_.c_str(), n);
        return;
    }

    l.outputAsAny(outlets_[n], s);
}

bool BaseObject::processAnyInlets(t_symbol* sel, const AtomList& lst)
{
    if (sel->s_name[0] != '_')
        return false;

    SymbolList::iterator it = std::find(inlets_s_.begin(), inlets_s_.end(), sel);
    if (it == inlets_s_.end())
        return false;

    size_t pos = std::distance(inlets_s_.begin(), it) + 1;
    processInlet(pos, lst);
    return true;
}

bool BaseObject::processAnyProps(t_symbol* sel, const AtomList& lst)
{
    if (sel->s_name[0] != '@')
        return false;

    Properties::iterator it = props_.find(sel);
    if (it == props_.end())
        return false;

    if (lst.empty()) {
        if (numOutlets() < 1)
            return true;

        anyTo(0, sel, it->second);
    } else {
        it->second = lst;
    }

    return true;
}

t_outlet* BaseObject::createOutlet(bool signal)
{
    t_outlet* out = outlet_new(pd_.owner, signal ? &s_signal : &s_anything);
    outlets_.push_back(out);
    return out;
}

void BaseObject::freeOutlets()
{
    OutletList::iterator it;
    for (it = outlets_.begin(); it != outlets_.end(); ++it)
        outlet_free(*it);
}

t_inlet* BaseObject::createInlet(float* v)
{
    t_inlet* in = floatinlet_new(pd_.owner, v);
    inlets_.push_back(in);
    return in;
}

t_inlet* BaseObject::createInlet(t_symbol** s)
{
    t_inlet* in = symbolinlet_new(pd_.owner, s);
    inlets_.push_back(in);
    return in;
}

void BaseObject::freeInlets()
{
    InletList::iterator it;
    for (it = inlets_.begin(); it != inlets_.end(); ++it)
        inlet_free(*it);
}

t_inlet* BaseObject::createInlet()
{
    char buf[MAXPDSTRING];
    sprintf(buf, "_inlet%zu", inlets_.size());
    t_symbol* id = gensym(buf);
    t_inlet* in = inlet_new(pd_.owner, &pd_.owner->ob_pd, &s_list, id);
    inlets_s_.push_back(id);
    inlets_.push_back(in);
    return in;
}

BaseObject::BaseObject(const PdArgs& args)
    : pd_(args)
{
}

BaseObject::~BaseObject()
{
    freeInlets();
    freeOutlets();
}
}
