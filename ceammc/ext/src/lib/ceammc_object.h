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

#include <iostream>
#include <map>
#include <sstream>
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
    t_symbol* receive_from_;

public:
    typedef AtomList (BaseObject::*GetterFn)() const;
    typedef void (BaseObject::*SetterFn)(const AtomList&);

public:
    BaseObject(const PdArgs& args);
    virtual ~BaseObject();

    inline AtomList& args() { return pd_.args; }
    inline const AtomList& args() const { return pd_.args; }
    void parseArguments();

    /**
     * Returns object class name as string.
     */
    inline std::string className() const { return pd_.className->s_name; }

    /**
     * Returns pointer to pd object struct, if you need manually call pd fuctions.
     */
    inline t_object* owner() { return pd_.owner; }

    /**
     * Dumps object info to Pd window
     */
    virtual void dump() const;

    /**
     * You should override this functions to react upon arrived messages.
     */
    virtual void onBang() {}
    virtual void onFloat(float) {}
    virtual void onSymbol(t_symbol*) {}
    virtual void onList(const AtomList&) {}
    virtual void onAny(t_symbol*, const AtomList&) {}

    /**
     * This function called when value come in inlet, except the first one
     * @param - inlet number, starting form 0
     * @param - incoming message
     */
    virtual void onInlet(size_t, const AtomList&) {}

    t_inlet* createInlet();

    /**
     * Creates float inlet - all incoming messages will change binded float value
     * @param v - pointer to binded float value
     * @return pointer to new inlet
     */
    t_inlet* createInlet(float* v);

    /**
     * Creates symbol inlet - all incoming messages will change binded symbol value
     * @param s - pointer to binded symbol value
     * @return pointer to new inlet
     */
    t_inlet* createInlet(t_symbol** s);

    /**
     * Returns number of inlets
     */
    size_t numInlets() const;

    /**
     * Creates outlet
     * @param signal - if true create signal outlet
     */
    t_outlet* createOutlet(bool signal = false);

    /**
     * Returns pointer to outlet specified by given index
     * @param n - outlet index. Starting from 0.
     * @return pointer to outlet or 0 if invalid index given.
     */
    t_outlet* outletAt(size_t n);

    /**
     * Returns number of outlets.
     */
    size_t numOutlets() const { return outlets_.size(); }

    void createProperty(Property* p);
    template <class T>
    void createCbProperty(const std::string& name,
        AtomList (T::*getter)() const,
        void (T::*setter)(const AtomList&) = 0)
    {
        CallbackProperty<T>* p = new CallbackProperty<T>(name, static_cast<T*>(this), getter, setter);
        createProperty(p);
    }
    bool hasProperty(t_symbol* key) const;

    /**
     * Outputs atom to specified outlet
     * @param n - outlet number
     * @param a - atom value
     */
    void atomTo(size_t n, const Atom& a);

    /**
     * Outputs bang to specified outlet
     * @param n - outlet number
     */
    void bangTo(size_t n);

    /**
     * Outputs float value to specified outlet
     * @param n - outlet number
     * @param v - float value
     */
    void floatTo(size_t n, float v);

    /**
     * Outputs symbol value to specified outlet
     * @param n - outlet number
     * @param s - symbol value
     */
    void symbolTo(size_t n, t_symbol* s);

    /**
     * Outputs list to specified outlet
     * @param n - outlet number
     * @param l - list value
     */
    void listTo(size_t n, const AtomList& l);

    /**
     * Outputs message to specified outlet
     * @param n - outlet number
     * @param msg - message value
     */
    void messageTo(size_t n, const Message& msg);
    void anyTo(size_t n, t_symbol* s, const Atom& a);
    void anyTo(size_t n, t_symbol* s, const AtomList& l);

    virtual bool processAnyInlets(t_symbol* sel, const AtomList& lst);
    virtual bool processAnyProps(t_symbol* sel, const AtomList& lst);
    virtual void anyDispatch(t_symbol* s, const AtomList& lst);

    void bindReceive(t_symbol* path);
    void unbindReceive();
    t_symbol* receive();

public:
    static t_symbol* tryGetPropKey(t_symbol* sel);

private:
    void freeInlets();
    void freeOutlets();
    void freeProps();
    AtomList propNumInlets();
    AtomList propNumOutlets();
    AtomList listAllProps() const;
};
}

#endif // CEAMMC_OBJECT_H
