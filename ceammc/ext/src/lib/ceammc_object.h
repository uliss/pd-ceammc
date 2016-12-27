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
#ifndef CEAMMC_OBJECT_H
#define CEAMMC_OBJECT_H

#include "ceammc_atomlist.h"
#include "ceammc_message.h"
#include "ceammc_property.h"

#include <map>
#include <string>
#include <vector>

namespace ceammc {

class PdArgs {
public:
    AtomList args;
    t_symbol* className;
    t_object* owner;

    PdArgs(const AtomList& lst, t_symbol* c, t_object* own)
        : args(lst)
        , className(c)
        , owner(own)
    {
    }
};

class BaseObject {
    PdArgs pd_;
    typedef std::vector<t_inlet*> InletList;
    typedef std::vector<t_outlet*> OutletList;
    typedef std::vector<t_symbol*> SymbolList;
    typedef std::map<t_symbol*, Property*> Properties;
    InletList inlets_;
    OutletList outlets_;
    SymbolList inlets_s_;
    Properties props_;

public:
    typedef AtomList (BaseObject::*GetterFn)();
    typedef void (BaseObject::*SetterFn)(const AtomList&);

public:
    BaseObject(const PdArgs& args);
    virtual ~BaseObject();

    inline AtomList& args() { return pd_.args; }
    inline const AtomList& args() const { return pd_.args; }
    inline std::string className() const { return pd_.className->s_name; }
    inline t_object* owner() { return pd_.owner; }

    /**
     * Dumps object info to Pd window
     */
    virtual void dump() const;

    virtual void onBang() {}
    virtual void onFloat(float) {}
    virtual void onSymbol(t_symbol*) {}
    virtual void onList(const AtomList&) {}
    virtual void onAny(t_symbol*, const AtomList&) {}
    void onInlet(size_t, const AtomList&) {}

    void anyDispatch(t_symbol* s, const AtomList& lst)
    {
        if (processAnyInlets(s, lst))
            return;

        if (processAnyProps(s, lst))
            return;

        onAny(s, lst);
    }

    t_inlet* createInlet();
    t_inlet* createInlet(float* v);
    t_inlet* createInlet(t_symbol** s);
    void freeInlets();
    size_t numInlets() const;

    t_outlet* createOutlet(bool signal = false);
    void freeOutlets();
    inline t_outlet* outletAt(size_t n);
    size_t numOutlets() const { return outlets_.size(); }

    void createProperty(Property* p);
    template <class T>
    void createCbProperty(const std::string& name,
        AtomList (T::*getter)(),
        void (T::*setter)(const AtomList&) = 0)
    {
        CallbackProperty<T>* p = new CallbackProperty<T>(name, static_cast<T*>(this), getter, setter);
        createProperty(p);
    }

    void atomTo(size_t n, const Atom& a);
    void bangTo(size_t n);
    void floatTo(size_t n, float v);
    void symbolTo(size_t n, t_symbol* s);
    void listTo(size_t n, const AtomList& l);
    void messageTo(size_t n, const Message& msg);
    void anyTo(size_t n, t_symbol* s, const Atom& a);
    void anyTo(size_t n, t_symbol* s, const AtomList& l);

    bool processAnyInlets(t_symbol* sel, const AtomList& lst);
    bool processAnyProps(t_symbol* sel, const AtomList& lst);

private:
    void freeProps();
    AtomList propNumInlets();
    AtomList propNumOutlets();
    AtomList listAllProps();
};
}

#endif // CEAMMC_OBJECT_H
