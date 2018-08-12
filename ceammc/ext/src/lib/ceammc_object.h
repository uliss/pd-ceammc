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
#include "ceammc_data.h"
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
    t_symbol* creationName;
    int flags;
    bool noDefaultInlet;
    bool mainSignalInlet;

    PdArgs(const AtomList& lst, t_symbol* c, t_object* own, t_symbol* alias)
        : args(lst)
        , className(c)
        , owner(own)
        , creationName(alias)
        , flags(0)
        , noDefaultInlet(false)
        , mainSignalInlet(false)
    {
    }

    bool hasDefaultSignalInlet() const
    {
        return mainSignalInlet && !noDefaultInlet;
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
    t_canvas* cnv_;

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
        ARG_BOOL,
        ARG_BYTE
    };

public:
    BaseObject(const PdArgs& args);
    virtual ~BaseObject();

    /**
     * Returns specified position argument (before property list)
     * @param pos - argument position
     * @param def - default value, if searched argument not exists
     */
    Atom positionalArgument(size_t pos, const Atom& def = Atom()) const;

    /**
     * Same as positionalArgument, but for t_float type
     */
    t_float positionalFloatArgument(size_t pos, t_float def = 0.f) const;

    /**
     * Same as positionalArgument, but for t_symbol* type
     */
    t_symbol* positionalSymbolArgument(size_t pos, t_symbol* def = 0) const;
    inline const AtomList& positionalArguments() const { return positional_args_; }

    /**
     * Parse initial constructor arguments and extract properties
     */
    virtual void parseProperties();

    /**
     * Method args checking
     */
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
    virtual void onBang();
    virtual void onFloat(float);
    virtual void onSymbol(t_symbol*);
    virtual void onList(const AtomList&);
    virtual void onData(const DataPtr&);
    virtual void onAny(t_symbol* s, const AtomList&);

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
     * Creates control outlet
     */
    t_outlet* createOutlet();

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
    bool setPropertyFromPositionalArg(Property* p, size_t n);

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

    /**
     * Sends data to specified outlet
     * @param n - output number
     * @param d - data pointer
     */
    virtual void dataTo(size_t n, const DataPtr& d);

    virtual bool processAnyInlets(t_symbol* sel, const AtomList& lst);

    /**
     * Used internally to get/set properties:
     *  - [@prop?( - output property value
     *  - [@prop VALUE( - set property value
     *
     * @param sel property name
     * @param lt property value
     * @return true if property request was successully finished
     *
     * @note override this method for custom property processing
     */
    virtual bool processAnyProps(t_symbol* sel, const AtomList& lst);

    /**
     * Main dispatcher of *any* messages. (Not bang, symbol, pointer, list or registered method)
     */
    virtual void anyDispatch(t_symbol* s, const AtomList& lst);

    /**
     * Bind object to listen global signal bus
     * @see unbindReceive()
     */
    void bindReceive(t_symbol* path);

    /**
     * Unbind object
     * @see bindReceive()
     */
    void unbindReceive();

    /**
     * Binded address
     */
    t_symbol* receive();

    /**
     * Returns pointer to parent canvas
     * @return pointer to canvas or NULL
     */
    t_canvas* canvas() { return cnv_; }
    const t_canvas* canvas() const { return cnv_; }

    /**
     * Returns pointer to root canvas (top window)
     */
    t_canvas* rootCanvas();
    t_canvas* rootCanvas() const;

    /**
     * Tries to find file by given filename
     * @param fname - filename (relative or absolute)
     * @return full path or empty string if not found
     */
    std::string findInStdPaths(const char* fname) const;

public:
    static t_symbol* tryGetPropKey(t_symbol* sel);

    /**
     * Checks if given filesystem path is absolute (starts with / on UNIX, and for ex. C:// on Windows)
     * @param ch - filepath
     */
    static bool isAbsolutePath(const char* path);

protected:
    void freeInlets();
    void freeOutlets();
    void freeProps();
    AtomList propNumInlets();
    AtomList propNumOutlets();
    AtomList listAllProps() const;
    const AtomList& args() const { return pd_.args; }
    void appendInlet(t_inlet* in);
    void appendOutlet(t_outlet* out);

private:
    void extractPositionalArguments();
};
}

#endif // CEAMMC_OBJECT_H
