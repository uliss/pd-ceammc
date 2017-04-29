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
#include "ceammc_format.h"
#include "ceammc_log.h"

#include <algorithm>
#include <cstdarg>
#include <cstring>

extern "C" {
#include "m_imp.h"
}

namespace ceammc {

t_outlet* BaseObject::outletAt(size_t n)
{
    if (n >= outlets_.size()) {
        OBJ_ERR << "invalid outlet index: " << n;
        return 0;
    }

    return outlets_[n];
}

void BaseObject::createProperty(Property* p)
{
    t_symbol* key = gensym(p->name().c_str());
    Properties::iterator it = props_.find(key);
    if (it != props_.end()) {
        // free previous
        if (p != it->second)
            delete it->second;
    }

    props_[key] = p;
}

bool BaseObject::hasProperty(t_symbol* key) const
{
    return props_.find(key) != props_.end();
}

bool BaseObject::hasProperty(const char* key) const
{
    return hasProperty(gensym(key));
}

Property* BaseObject::property(t_symbol* key)
{
    Properties::iterator it = props_.find(key);
    return it == props_.end() ? 0 : it->second;
}

Property* BaseObject::property(const char* key)
{
    return property(gensym(key));
}

bool BaseObject::setProperty(t_symbol* key, const AtomList& v)
{
    Property* p = property(key);
    if (p == 0 || p->readonly())
        return false;

    return p->set(v);
}

bool BaseObject::setProperty(const char* key, const AtomList& v)
{
    return setProperty(gensym(key), v);
}

void BaseObject::bangTo(size_t n)
{
    if (n >= outlets_.size()) {
        OBJ_ERR << "invalid outlet index: " << n;
        return;
    }
    outlet_bang(outlets_[n]);
}

void BaseObject::floatTo(size_t n, float v)
{
    if (n >= outlets_.size()) {
        OBJ_ERR << "invalid outlet index: " << n;
        return;
    }
    outlet_float(outlets_[n], v);
}

void BaseObject::symbolTo(size_t n, t_symbol* s)
{
    if (n >= outlets_.size()) {
        OBJ_ERR << "invalid outlet index: " << n;
        return;
    }
    outlet_symbol(outlets_[n], s);
}

void BaseObject::atomTo(size_t n, const Atom& a)
{
    if (n >= outlets_.size()) {
        OBJ_ERR << "invalid outlet index: " << n;
        return;
    }

    a.output(outlets_[n]);
}

void BaseObject::listTo(size_t n, const AtomList& l)
{
    if (n >= outlets_.size()) {
        OBJ_ERR << "invalid outlet index: " << n;
        return;
    }

    l.output(outlets_[n]);
}

void BaseObject::messageTo(size_t n, const Message& msg)
{
    if (n >= outlets_.size()) {
        OBJ_ERR << "invalid outlet index: " << n;
        return;
    }

    msg.output(outlets_[n]);
}

void BaseObject::anyTo(size_t n, const AtomList& l)
{
    if (n >= outlets_.size()) {
        OBJ_ERR << "invalid outlet index: " << n;
        return;
    }

    l.outputAsAny(outlets_[n]);
}

void BaseObject::anyTo(size_t n, t_symbol* s, const Atom& a)
{
    if (n >= outlets_.size()) {
        OBJ_ERR << "invalid outlet index: " << n;
        return;
    }

    a.outputAsAny(outlets_[n], s);
}

void BaseObject::anyTo(size_t n, t_symbol* s, const AtomList& l)
{
    if (n >= outlets_.size()) {
        OBJ_ERR << "invalid outlet index: " << n;
        return;
    }

    l.outputAsAny(outlets_[n], s);
}

bool BaseObject::processAnyInlets(t_symbol* sel, const AtomList& lst)
{
    if (sel->s_name[0] != '_')
        return false;

    SymbolList::iterator it = std::find(inlets_s_.begin(), inlets_s_.end(), sel);
    if (it == inlets_s_.end()) {
        OBJ_ERR << "invalid inlet: " << sel->s_name;
        return false;
    }

    size_t pos = std::distance(inlets_s_.begin(), it) + 1;
    onInlet(pos, lst);
    return true;
}

bool BaseObject::processAnyProps(t_symbol* sel, const AtomList& lst)
{
    if (sel->s_name[0] != '@')
        return false;

    t_symbol* get_key = tryGetPropKey(sel);
    if (get_key != 0)
        sel = get_key;

    Properties::iterator it = props_.find(sel);
    if (it == props_.end()) {
        OBJ_ERR << "invalid property: " << sel->s_name;
        return false;
    }

    if (get_key != 0) {
        if (numOutlets() < 1)
            return true;

        anyTo(0, get_key, it->second->get());
    } else {
        it->second->set(lst);
    }

    return true;
}

void BaseObject::freeProps()
{
    Properties::iterator it;
    for (it = props_.begin(); it != props_.end(); ++it)
        delete it->second;

    props_.erase(props_.begin(), props_.end());
}

AtomList BaseObject::propNumInlets()
{
    return listFrom(numInlets());
}

AtomList BaseObject::propNumOutlets()
{
    return listFrom(numOutlets());
}

AtomList BaseObject::listAllProps() const
{
    AtomList res;
    Properties::const_iterator it;
    for (it = props_.begin(); it != props_.end(); ++it) {
        if (it->first == gensym("@*"))
            continue;

        res.append(Atom(it->first));
    }

    return res;
}

void BaseObject::extractPositionalArguments()
{
    int idx = pd_.args.findPos(isProperty);
    if (idx < 0)
        idx = pd_.args.size();

    positional_args_ = pd_.args.slice(0, idx);
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

size_t BaseObject::numInlets() const
{
    return pd_.owner ? static_cast<size_t>(obj_ninlets(pd_.owner)) : 0;
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
    , receive_from_(0)
{
    createCbProperty("@*", &BaseObject::listAllProps);

    extractPositionalArguments();
}

BaseObject::~BaseObject()
{
    unbindReceive();
    freeInlets();
    freeOutlets();
    freeProps();
}

Atom BaseObject::positionalArgument(size_t pos, const Atom& def) const
{
    return pos < positional_args_.size() ? positional_args_[pos] : def;
}

t_float BaseObject::positionalFloatArgument(size_t pos, t_float def) const
{
    return pos < positional_args_.size() ? positional_args_[pos].asFloat(def) : def;
}

t_symbol* BaseObject::positionalSymbolArgument(size_t pos, t_symbol* def) const
{
    if (pos >= positional_args_.size())
        return def;

    return positional_args_[pos].isSymbol() ? positional_args_[pos].asSymbol() : def;
}

void BaseObject::parseProperties()
{
    std::deque<AtomList> p = pd_.args.properties();
    for (size_t i = 0; i < p.size(); i++) {
        if (p[i].size() < 1) {
            continue;
        }

        t_symbol* pname = p[i][0].asSymbol();
        if (!hasProperty(pname)) {
            OBJ_ERR << "unknown property in argument list: " << pname->s_name;
            continue;
        }

        props_[pname]->set(p[i].slice(1));
    }
}

bool BaseObject::checkArg(const Atom& atom, BaseObject::ArgumentType type, int pos) const
{
#define ARG_ERROR(msg)           \
    {                            \
        std::ostringstream os;   \
        os << msg;               \
        if (pos >= 0)            \
            os << " at " << pos; \
        OBJ_ERR << os.str();     \
        return false;            \
    }

    switch (type) {
    case ARG_FLOAT:
        if (!atom.isFloat())
            ARG_ERROR("float expected");
        break;
    case ARG_SYMBOL:
        if (!atom.isSymbol())
            ARG_ERROR("symbol expected");
        break;
    case ARG_PROPERTY:
        if (!atom.isProperty())
            ARG_ERROR("property expected");
        break;
    case ARG_SNONPROPERTY:
        if (!atom.isSymbol() || atom.isProperty())
            ARG_ERROR("symbol and non property expected");
        break;
    case ARG_INT:
        if (!atom.isInteger())
            ARG_ERROR("integer expected");
        break;
    case ARG_NATURAL:
        if (!atom.isNatural())
            ARG_ERROR("natural expected");
        break;
    case ARG_BOOL:
        if (!atom.isFloat())
            ARG_ERROR("boolean expected");

        if (atom.asFloat() != 0.f && atom.asFloat() != 1.f)
            ARG_ERROR("only 1 or 0 accepted");

        break;
    }

    return true;

#undef ARG_ERROR
}

static const char* to_string(BaseObject::ArgumentType a)
{
    static const char* names[] = {
        "float",
        "int",
        "natural",
        "symbol",
        "property",
        "non-property symbol",
        "bool"
    };

    return names[a];
}

bool BaseObject::checkArgs(const AtomList& lst, ArgumentType a1, t_symbol* method) const
{
    if (lst.size() < 1
        || !checkArg(lst[0], a1, 0)) {

        if (method)
            OBJ_ERR << "Usage: " << method->s_name << " " << to_string(a1);

        return false;
    }

    return true;
}

bool BaseObject::checkArgs(const AtomList& lst, BaseObject::ArgumentType a1,
    BaseObject::ArgumentType a2, t_symbol* method) const
{
    if (lst.size() < 2
        || !checkArg(lst[0], a1, 0)
        || !checkArg(lst[1], a2, 1)) {

        if (method)
            OBJ_ERR << "Usage: " << method->s_name
                    << " " << to_string(a1)
                    << " " << to_string(a2);

        return false;
    }

    return true;
}

bool BaseObject::checkArgs(const AtomList& lst, BaseObject::ArgumentType a1,
    BaseObject::ArgumentType a2, BaseObject::ArgumentType a3, t_symbol* method) const
{
    if (lst.size() < 3
        || !checkArg(lst[0], a1, 0)
        || !checkArg(lst[1], a2, 1)
        || !checkArg(lst[2], a3, 2)) {

        if (method)
            OBJ_ERR << "Usage: " << method->s_name
                    << " " << to_string(a1)
                    << " " << to_string(a2)
                    << " " << to_string(a3);

        return false;
    }

    return true;
}

bool BaseObject::checkArgs(const AtomList& lst, BaseObject::ArgumentType a1,
    BaseObject::ArgumentType a2, BaseObject::ArgumentType a3,
    BaseObject::ArgumentType a4, t_symbol* method) const
{

    if (lst.size() < 4
        || !checkArg(lst[0], a1, 0)
        || !checkArg(lst[1], a2, 1)
        || !checkArg(lst[2], a3, 2)
        || !checkArg(lst[3], a4, 3)) {

        if (method)
            OBJ_ERR << "Usage: " << method->s_name
                    << " " << to_string(a1)
                    << " " << to_string(a2)
                    << " " << to_string(a3)
                    << " " << to_string(a4);

        return false;
    }

    return true;
}

void BaseObject::dump() const
{
    post("[%s] inlets: %zu", className().c_str(), numInlets());
    post("[%s] outlets: %zu", className().c_str(), numOutlets());

    Properties::const_iterator it;
    for (it = props_.begin(); it != props_.end(); ++it) {
        if (!it->second->visible())
            continue;

        post("[%s] property: %s = %s",
            className().c_str(),
            it->first->s_name,
            to_string(it->second->get()).c_str());
    }
}

void BaseObject::anyDispatch(t_symbol* s, const AtomList& lst)
{
    if (processAnyInlets(s, lst))
        return;

    if (processAnyProps(s, lst))
        return;

    onAny(s, lst);
}

void BaseObject::bindReceive(t_symbol* path)
{
    if (path == 0) {
        OBJ_ERR << "attempt to receive from empty path";
        return;
    }

    if (receive_from_ != path)
        unbindReceive();

    receive_from_ = path;
    pd_bind(&owner()->te_g.g_pd, path);
}

void BaseObject::unbindReceive()
{
    if (receive_from_) {
        pd_unbind(&owner()->te_g.g_pd, receive_from_);
        receive_from_ = 0;
    }
}

t_symbol* BaseObject::receive()
{
    return receive_from_;
}

t_symbol* BaseObject::tryGetPropKey(t_symbol* sel)
{
    char buf[MAXPDSTRING] = { 0 };
    t_symbol* res = 0;
    const char* s_name = sel->s_name;
    const size_t last_char_idx = strlen(s_name) - 1;
    if (s_name[last_char_idx] == '?') {
        memcpy(&buf, s_name, last_char_idx);
        buf[last_char_idx] = '\0';
        res = gensym(buf);
    }

    return res;
}
}
