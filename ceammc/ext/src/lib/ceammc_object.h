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
    const PdArgs pd_;
    typedef std::vector<t_inlet*> InletList;
    typedef std::vector<t_outlet*> OutletList;
    typedef std::vector<t_symbol*> SymbolList;
    typedef std::map<t_symbol*, Property*> Properties;
    InletList inlets_;
    OutletList outlets_;
    SymbolList inlets_s_;
    Properties props_;
    AtomList positional_args_;
    t_symbol* receive_from_;

public:
    typedef AtomList (BaseObject::*GetterFn)() const;
    typedef void (BaseObject::*SetterFn)(const AtomList&);

    /**
     * @note if adding new type: see static to_string in ceammc_object.cpp
     */
    enum ArgumentType {
        ARG_FLOAT = 0,
        ARG_INT,
        ARG_NATURAL,
        ARG_SYMBOL,
        ARG_PROPERTY,
        ARG_SNONPROPERTY,
        ARG_BOOL
    };

public:
    BaseObject(const PdArgs& args);
    virtual ~BaseObject();

    Atom positionalArgument(size_t pos, const Atom& def = Atom()) const;
    t_float positionalFloatArgument(size_t pos, t_float def = 0.f) const;
    t_symbol* positionalSymbolArgument(size_t pos, t_symbol* def = 0) const;
    inline const AtomList& positionalArguments() const { return positional_args_; }
    virtual void parseProperties();

    bool checkArg(const Atom& atom, ArgumentType type, int pos = -1) const;
    bool checkArgs(const AtomList& lst, ArgumentType a1, t_symbol* method = 0) const;
    bool checkArgs(const AtomList& lst, ArgumentType a1, ArgumentType a2, t_symbol* method = 0) const;
    bool checkArgs(const AtomList& lst, ArgumentType a1, ArgumentType a2, ArgumentType a3, t_symbol* method = 0) const;
    bool checkArgs(const AtomList& lst, ArgumentType a1, ArgumentType a2, ArgumentType a3, ArgumentType a4, t_symbol* method = 0) const;

    /**
     * Returns object class name as string.
     */
    inline std::string className() const { return pd_.className->s_name; }

    /**
     * Returns pointer to pd object struct, if you need manually call pd fuctions.
     */
    inline t_object* owner() const { return pd_.owner; }

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
    bool hasProperty(const char* key) const;
    Property* property(t_symbol* key);
    Property* property(const char* key);
    bool setProperty(t_symbol* key, const AtomList& v);
    bool setProperty(const char* key, const AtomList& v);

    /**
     * Outputs atom to specified outlet
     * @param n - outlet number
     * @param a - atom value
     */
    virtual void atomTo(size_t n, const Atom& a);

    /**
     * Outputs bang to specified outlet
     * @param n - outlet number
     */
    virtual void bangTo(size_t n);

    /**
     * Outputs float value to specified outlet
     * @param n - outlet number
     * @param v - float value
     */
    virtual void floatTo(size_t n, float v);

    /**
     * Outputs symbol value to specified outlet
     * @param n - outlet number
     * @param s - symbol value
     */
    virtual void symbolTo(size_t n, t_symbol* s);

    /**
     * Outputs list to specified outlet
     * @param n - outlet number
     * @param l - list value
     */
    virtual void listTo(size_t n, const AtomList& l);

    /**
     * Outputs message to specified outlet
     * @param n - outlet number
     * @param msg - message value
     */
    virtual void messageTo(size_t n, const Message& msg);

    virtual void anyTo(size_t n, const AtomList& l);
    virtual void anyTo(size_t n, t_symbol* s, const Atom& a);
    virtual void anyTo(size_t n, t_symbol* s, const AtomList& l);

    virtual bool processAnyInlets(t_symbol* sel, const AtomList& lst);
    virtual bool processAnyProps(t_symbol* sel, const AtomList& lst);
    virtual void anyDispatch(t_symbol* s, const AtomList& lst);

    void bindReceive(t_symbol* path);
    void unbindReceive();
    t_symbol* receive();

public:
    static t_symbol* tryGetPropKey(t_symbol* sel);

protected:
    void freeInlets();
    void freeOutlets();
    void freeProps();
    AtomList propNumInlets();
    AtomList propNumOutlets();
    AtomList listAllProps() const;
    const AtomList& args() const { return pd_.args; }

private:
    void extractPositionalArguments();
};
}

#endif // CEAMMC_OBJECT_H
