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
#include "ceammc_object_info.h"
#include "ceammc_property.h"

#include <array>
#include <cstdint>
#include <initializer_list>
#include <iostream>
#include <map>
#include <sstream>
#include <string>
#include <unordered_map>
#include <vector>

namespace ceammc {

#define OBJ_ERR Error(this).stream()
#define OBJ_DBG Debug(this).stream()
#define OBJ_LOG Log(this).stream()
#define OBJ_POST Post(this).stream()
#define METHOD_ERR(s) OBJ_ERR << "[" << s->s_name << "( "
#define METHOD_DBG(s) OBJ_DBG << "[" << s->s_name << "( "
#define METHOD_LOG(s) OBJ_LOG << "[" << s->s_name << "( "
#define METHOD_POST(s) OBJ_POST << "[" << s->s_name << "( "

class PdArgs {
public:
    AtomList args;
    t_symbol* className;
    t_object* owner;
    t_symbol* creationName;
    int flags = { 0 };
    bool noDefaultInlet : 1;
    bool mainSignalInlet : 1;
    bool noArgsDataParsing : 1;
    bool ignoreDataParseErrors : 1;
    bool parsePosPropsOnly : 1;

    PdArgs(const AtomList& lst, t_symbol* c, t_object* own, t_symbol* alias)
        : args(lst)
        , className(c)
        , owner(own)
        , creationName(alias)
        , flags(0)
        , noDefaultInlet(false)
        , mainSignalInlet(false)
        , noArgsDataParsing(false)
        , ignoreDataParseErrors(false)
        , parsePosPropsOnly(false)
    {
    }

    bool hasDefaultSignalInlet() const
    {
        return mainSignalInlet && !noDefaultInlet;
    }
};

class BaseObject {
    const PdArgs pd_;
    using InletList = std::vector<t_inlet*>;
    using OutletList = std::vector<t_outlet*>;
    using Properties = std::vector<Property*>;

    InletList inlets_;
    OutletList outlets_;
    Properties props_;
    AtomListView pos_args_unparsed_;
    AtomList pos_args_parsed_;
    t_symbol* receive_from_;
    t_canvas* cnv_;

    // no copy
    BaseObject(const BaseObject&) = delete;
    // no assign
    BaseObject& operator=(const BaseObject&) = delete;

public:
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
    /**
     * Base class for ceammc objects
     * @param args
     */
    BaseObject(const PdArgs& args);

    /**
     * Destructor
     */
    virtual ~BaseObject();

    template <size_t def, size_t min, size_t max>
    size_t positionalConstant(size_t pos) const
    {
        static_assert(min < max, "min < max expected");
        static_assert(min <= def, "min <= def expected");
        static_assert(def <= max, "def <= max expected");

        return positionalConstantP(pos, def, min, max);
    }

private:
    size_t positionalConstantP(size_t pos, size_t def, size_t min, size_t max) const;

public:
    /**
     * Return raw object creation arguments unparsed (no $* substitution)
     * @note: all args returned, you are really seldom need this
     * @see parsedPosArgs() or unparsedPosArgs()
     */
    AtomListView binbufArgs() const;

    /**
     * Unparsed positional arguments
     * @
     */
    const AtomListView& unparsedPosArgs() const { return pos_args_unparsed_; }

    /**
     * Unparsed positional arguments
     */
    const AtomList& parsedPosArgs() const { return pos_args_parsed_; }

    /**
     * Parse initial constructor arguments and extract properties
     */
    virtual void parseProperties();

    /**
     * Updates PropertyInfo defaults value, that can be available after parse step
     */
    virtual void updatePropertyDefaults();

    /**
     * called when object init is done
     */
    virtual void initDone();

    /**
     * Method args checking
     */
    bool checkArg(const Atom& atom, ArgumentType type, int pos = -1) const;
    bool checkArgs(const AtomListView& lst, ArgumentType a1, t_symbol* method = 0) const;
    bool checkArgs(const AtomListView& lst, ArgumentType a1, ArgumentType a2, t_symbol* method = 0) const;
    bool checkArgs(const AtomListView& lst, ArgumentType a1, ArgumentType a2, ArgumentType a3, t_symbol* method = 0) const;
    bool checkArgs(const AtomListView& lst, ArgumentType a1, ArgumentType a2, ArgumentType a3, ArgumentType a4, t_symbol* method = 0) const;

    /**
     * Returns object class name
     */
    t_symbol* className() const { return pd_.className; }

    /**
     * Returns object class pointer
     */
    t_class* classPointer() const { return pd_.owner->te_g.g_pd; }

    /**
     * Returns pointer to pd object struct, if you need manually call pd fuctions.
     */
    t_object* owner() const { return pd_.owner; }

    /**
     * Dumps object info to Pd window
     */
    virtual void dump() const;

    /**
     * Outputs all properties name
     * @example on message [@*?( outputs message [@* @prop1 @prop2 etc..(
     */
    void queryPropNames();

    /**
     * You should override this functions to react upon arrived messages.
     */
    virtual void onBang();
    virtual void onFloat(t_float);
    virtual void onSymbol(t_symbol*);
    virtual void onList(const AtomList&);
    virtual void onData(const Atom&);
    virtual void onAny(t_symbol* s, const AtomListView&);

    /**
     * This function called when value come in inlet, except the first one
     * @param - inlet number, starting form 0
     * @param - incoming message
     */
    virtual void onInlet(size_t, const AtomList&) {}

    /**
     * This function called on object click  (should be enabled in factory)
     * @param xpos - relative mouse x-pos
     * @param ypos - relative mouse y-pos
     * @param shift - if shift modifier is pressed
     * @param ctrl - if control modifier is pressed
     * @param alt - if alt modifier is pressed
     */
    virtual void onClick(t_floatarg xpos, t_floatarg ypos, t_floatarg shift, t_floatarg ctrl, t_floatarg alt);

    /**
     * called when loaded
     */
    virtual void onLoadBang() {}

    /**
     * called when loaded but not yet connected to parent patch
     */
    virtual void onInitBang() {}

    /**
     * called when about to close
     */
    virtual void onCloseBang() {}

    /**
     * Creates inlet
     * @return pointer
     */
    t_inlet* createInlet();

    /**
     * Creates float inlet - all incoming messages will change binded float value
     * @param v - pointer to binded float value
     * @return pointer to new inlet
     */
    t_inlet* createInlet(t_float* v);

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
     * Reserves space for inlets to reduce memory reallocations
     */
    void reserveInlets(size_t n) { inlets_.reserve(n); }

    /**
     * Inlet description
     * @param n - inlet index
     * @return inlet description constant string pointer, 0 if not exists
     */
    virtual const char* annotateInlet(size_t n) const;

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

    /**
     * Reserves space for outlets to reduce memory reallocations
     */
    void reserveOutlets(size_t n) { outlets_.reserve(n); }

    /**
     * Outlet description
     * @param n - outlet index
     * @return outlet description constant string pointer, 0 if not exists
     */
    virtual const char* annotateOutlet(size_t n) const;

    /**
     * Adds property to obejct and takes owner ship on it
     * @p - pointer to property
     * @return pointer to property
     */
    Property* addProperty(Property* p);

    /**
     * Creates callback property
     */
    template <class T>
    Property* createCbProperty(const std::string& name,
        AtomList (T::*getter)() const,
        void (T::*setter)(const AtomList&) = nullptr)
    {
        return createCbListProperty(
            name,
            [this, getter]() -> AtomList { return (static_cast<T*>(this)->*getter)(); },
            setter ? [this, setter](const AtomList& l) -> bool {
                (static_cast<T*>(this)->*setter)(l);
                return true;
            }
                   : PropertyListSetter());
    }

    /**
     * Create callback float property
     * @return pointer to created prperty
     */
    Property* createCbFloatProperty(const std::string& name,
        PropertyFloatGetter g,
        PropertyFloatSetter s = nullptr);

    /**
     * Create callback int property
     * @return pointer to created prperty
     */
    Property* createCbIntProperty(const std::string& name,
        PropertyIntGetter g,
        PropertyIntSetter s = nullptr);

    /**
     * Create callback bool property
     * @return pointer to created prperty
     */
    Property* createCbBoolProperty(const std::string& name,
        PropertyBoolGetter g,
        PropertyBoolSetter s = nullptr);

    /**
     * Create callback symbol property
     * @return pointer to created prperty
     */
    Property* createCbSymbolProperty(const std::string& name,
        PropertySymbolGetter g,
        PropertySymbolSetter s = nullptr);

    /**
     * Create callback atom property
     * @return pointer to created prperty
     */
    Property* createCbAtomProperty(const std::string& name,
        PropertyAtomGetter g,
        PropertyAtomSetter s = nullptr);

    /**
     * Create callback list property
     * @return pointer to created prperty
     */
    Property* createCbListProperty(const std::string& name,
        PropertyListGetter g,
        PropertyListSetter s = nullptr);

    /**
     * Check if object has specified property
     * @param key - property name
     * @return true on success, false on error
     */
    bool hasProperty(t_symbol* key) const;
    inline bool hasProperty(const char* key) const { return hasProperty(gensym(key)); }

    /**
     * Get pointer to specified property
     * @param key - property name
     * @return pointer to property on success, nullptr if not found
     */
    Property* property(t_symbol* key);
    Property* property(const char* key) { return property(gensym(key)); }
    const Property* property(t_symbol* key) const;

    /**
     * Set property value
     * @param key - property name
     * @param v - property value
     * @return false on error
     */
    bool setProperty(t_symbol* key, const AtomListView& v);
    bool setProperty(const char* key, const AtomListView& v);

    /**
     * Get list of object properties
     */
    inline const Properties& properties() const { return props_; }

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
     * Outputs 1 or 0 to specified outlet
     * @n - outlet number
     * @v - bool value
     */
    virtual void boolTo(size_t n, bool v);

    /**
     * Outputs float value to specified outlet
     * @param n - outlet number
     * @param v - t_float value
     */
    virtual void floatTo(size_t n, t_float v);

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
     * Outputs list view to specified outlet
     * @param n - outlet number
     * @param v - listview
     */
    virtual void listTo(size_t n, const AtomListView& v);

    /**
     * Outputs message to specified outlet
     * @param n - outlet number
     * @param msg - message value
     */
    virtual void messageTo(size_t n, const Message& msg);

    virtual void anyTo(size_t n, const AtomList& l);
    virtual void anyTo(size_t n, const AtomListView& l);
    virtual void anyTo(size_t n, t_symbol* s, const Atom& a);
    virtual void anyTo(size_t n, t_symbol* s, const AtomList& l);
    virtual void anyTo(size_t n, t_symbol* s, const AtomListView& l);

    /**
     * main secondary inlets dispatcher
     * @param sel - inlet selector
     * @param lst - inlet input data
     * @return true -  if message was dispatched to inlet and should not be processed anymore
     *         false - if not valid inlet message and should be processed further
     */
    virtual bool processAnyInlets(t_symbol* sel, const AtomListView& lst);

    /**
     * Used internally to get/set properties:
     *  - [@prop?( - output property value
     *  - [@prop VALUE( - set property value
     *
     * @param sel property name
     * @param lt property value
     * @return true  - if message was successully dispatched to property,
     *         false - if message message should be proceeded further
     *
     * @note override this method for custom property processing
     */
    virtual bool processAnyProps(t_symbol* sel, const AtomListView& lst);

    /**
     * Various load(close)bang dispatcher
     * @param action
     */
    void dispatchLoadBang(int action);

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
     * Check if patch is loading
     */
    bool isPatchLoading() const;

    /**
     * Tries to find file by given filename
     * @param fname - filename (relative or absolute)
     * @return full path or empty string if not found
     */
    std::string findInStdPaths(const char* fname) const;

public:
    /**
     * @brief tryGetPropKey - return property name for property query name: @prop? -> @prop
     * @param sel - property query, like @prop?
     * @return @prop or nullptr on error
     */
    static t_symbol* tryGetPropKey(t_symbol* sel);

    /**
     * Checks if given filesystem path is absolute (starts with / on UNIX, and for ex. C:// on Windows)
     * @param ch - filepath
     */
    static bool isAbsolutePath(const char* path);

    using XletInfo = std::vector<std::string>;
    using XletMap = std::map<t_class*, XletInfo>;

    static void addInletInfo(t_class* c, const std::string& txt);
    static void addOutletInfo(t_class* c, const std::string& txt);

    static void setInletsInfo(t_class* c, const XletInfo& l);
    static void setOutletsInfo(t_class* c, const XletInfo& l);

    static void initInletDispatchNames();

protected:
    void freeInlets();
    void freeOutlets();
    void freeProps();
    const AtomList& args() const { return pd_.args; }
    void appendInlet(t_inlet* in);
    void appendOutlet(t_outlet* out);
    bool queryProperty(t_symbol* key, AtomList& res) const;

    static const size_t MAX_XLETS_NUM = 255;

private:
    void extractPositionalArguments();

    static XletMap inlet_info_map;
    static XletMap outlet_info_map;
    static std::array<t_symbol*, MAX_XLETS_NUM> inlet_dispatch_names;
};
}

#endif // CEAMMC_OBJECT_H
