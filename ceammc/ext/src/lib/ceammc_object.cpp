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
#include "ceammc_convert.h"
#include "ceammc_data.h"
#include "ceammc_datatypes.h"
#include "ceammc_deprecated.h"
#include "ceammc_format.h"
#include "ceammc_log.h"
#include "ceammc_object_info.h"
#include "ceammc_output.h"
#include "ceammc_platform.h"
#include "ceammc_property_callback.h"
#include "ceammc_property_enum.h"
#include "datatype_string.h"
#include "fmt/format.h"
#include "lex/parser_strings.h"

#include <cstdarg>
#include <cstring>

#include <algorithm>
#include <boost/range/size.hpp>

extern "C" {
#include "g_canvas.h"
#include "m_imp.h"
}

namespace ceammc {

static inline char int2hex_char(uint8_t v)
{
    if (v < 0xA)
        return '0' + v;
    else
        return 'A' + (v - 0xA);
}

static inline uint8_t hex_char2int(char c)
{
    if (c < 'A')
        return 0x0F & (c - '0');
    else
        return 0x0F & ((c - 'A') + 0x0A);
}

static inline void int2hex(uint8_t v, char* buf)
{
    buf[0] = int2hex_char(v >> 4);
    buf[1] = int2hex_char(v & 0x0F);
}

static inline uint8_t hex2int(const char buf[2])
{
    return (hex_char2int(buf[0]) << 4) | (hex_char2int(buf[1]));
}

// static init
BaseObject::XletMap BaseObject::inlet_info_map;
BaseObject::XletMap BaseObject::outlet_info_map;
std::array<t_symbol*, BaseObject::MAX_XLETS_NUM> BaseObject::inlet_dispatch_names = { 0 };

t_outlet* BaseObject::outletAt(size_t n)
{
    if (n >= outlets_.size()) {
        OBJ_ERR << "invalid outlet index: " << n;
        return nullptr;
    }

    return outlets_[n];
}

void BaseObject::freeOutlets()
{
    for (auto x : outlets_)
        outlet_free(x);

    outlets_.clear();
}

void BaseObject::clearOutlets()
{
    auto cnv = canvas();
    auto obj = owner();

    if (cnv && obj) {
        //        for (auto* x : outlets_)
        //            canvas_deletelinesforio(cnv, obj, 0, x);
    }
}

const char* BaseObject::annotateOutlet(size_t n) const
{
    auto it = outlet_info_map.find(classPointer());
    if (it == outlet_info_map.end())
        return nullptr;

    if (n >= it->second.size())
        return nullptr;

    if (it->second[n].empty())
        return nullptr;

    return it->second[n].c_str();
}

Property* BaseObject::addProperty(Property* p)
{
    if (!p) {
        OBJ_ERR << "null property pointer";
        return p;
    }

    // find property with same name
    auto it = std::find_if(props_.begin(), props_.end(), [p](Property* x) { return x->name() == p->name(); });

    if (it != props_.end()) {
        if (*it == p) {
            OBJ_ERR << "property double insertion: " << p->name();
        } else {
            OBJ_LOG << "replacing property: " << p->name();
            delete *it;
            *it = p;
        }
    } else
        props_.push_back(p);

    p->setOwner(owner());
    return p;
}

Property* BaseObject::createCbFloatProperty(const char* name, PropertyFloatGetter g, PropertyFloatSetter s)
{
    return addProperty(new CallbackProperty(name, g, s));
}

Property* BaseObject::createCbIntProperty(const char* name, PropertyIntGetter g, PropertyIntSetter s)
{
    return addProperty(new CallbackProperty(name, g, s));
}

Property* BaseObject::createCbBoolProperty(const char* name, PropertyBoolGetter g, PropertyBoolSetter s)
{
    return addProperty(new CallbackProperty(name, g, s));
}

Property* BaseObject::createCbSymbolProperty(const char* name, PropertySymbolGetter g, PropertySymbolSetter s)
{
    return addProperty(new CallbackProperty(name, g, s));
}

Property* BaseObject::createCbAtomProperty(const char* name, PropertyAtomGetter g, PropertyAtomSetter s)
{
    return addProperty(new CallbackProperty(name, g, s));
}

Property* BaseObject::createCbListProperty(const char* name, PropertyListGetter g, PropertyListSetter s)
{
    return addProperty(new CallbackProperty(name, g, s));
}

bool BaseObject::hasProperty(t_symbol* key) const
{
    auto end = props_.end();
    return end != std::find_if(props_.begin(), end, [key](Property* p) { return p->name() == key; });
}

Property* BaseObject::property(t_symbol* key)
{
    auto end = props_.end();
    auto it = std::find_if(props_.begin(), end, [key](Property* p) { return p->name() == key; });
    return (it == end) ? nullptr : *it;
}

const Property* BaseObject::property(t_symbol* key) const
{
    auto end = props_.end();
    auto it = std::find_if(props_.begin(), end, [key](Property* p) { return p->name() == key; });
    return (it == end) ? nullptr : *it;
}

bool BaseObject::setProperty(t_symbol* key, const AtomListView& v)
{
    Property* p = property(key);
    if (!p || !p->isReadWrite())
        return false;

    return p->set(v);
}

bool BaseObject::setProperty(const char* key, const AtomListView& v)
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

void BaseObject::boolTo(size_t n, bool v)
{
    floatTo(n, v ? 1 : 0);
}

void BaseObject::floatTo(size_t n, t_float v)
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

    outletAtom(outlets_[n], a);
}

void BaseObject::listTo(size_t n, const AtomListView& lv)
{
    if (n >= outlets_.size()) {
        OBJ_ERR << "invalid outlet index: " << n;
        return;
    }

    outletAtomList(outlets_[n], lv);
}

void BaseObject::messageTo(size_t n, const Message& msg)
{
    if (n >= outlets_.size()) {
        OBJ_ERR << "invalid outlet index: " << n;
        return;
    }

    msg.output(outlets_[n]);
}

void BaseObject::anyTo(size_t n, const AtomListView& lv)
{
    if (n >= outlets_.size()) {
        OBJ_ERR << "invalid outlet index: " << n;
        return;
    }

    if (!outletAny(outlets_[n], lv))
        OBJ_ERR << "invalid message: " << lv;
}

void BaseObject::anyTo(size_t n, t_symbol* s, const Atom& a)
{
    if (n >= outlets_.size()) {
        OBJ_ERR << "invalid outlet index: " << n;
        return;
    }

    outletAny(outlets_[n], s, a);
}

void BaseObject::anyTo(size_t n, t_symbol* s, const AtomListView& l)
{
    if (n >= outlets_.size()) {
        OBJ_ERR << "invalid outlet index: " << n;
        return;
    }

    outletAny(outlets_[n], s, l);
}

t_inlet* BaseObject::createInlet()
{
    if (inlet_dispatch_names[0] == nullptr)
        initInletDispatchNames();

    const uint8_t N = inlets_.size();
    assert(N < MAX_XLETS_NUM);

    t_inlet* in = inlet_new(pd_.owner, &pd_.owner->ob_pd, &s_list, inlet_dispatch_names[N]);
    inlets_.push_back(in);
    return in;
}

bool BaseObject::processAnyInlets(t_symbol* sel, const AtomListView& lv)
{
    // format '_:%02x'
    const bool ok = sel->s_name[0] == '_'
        && sel->s_name[1] == ':'
        && sel->s_name[2] != '\0' // inlet index in ascii-hex
        && sel->s_name[3] != '\0' // ...
        && sel->s_name[4] == '\0';

    if (!ok)
        return false;

    const uint8_t N = hex2int(&sel->s_name[2]);

    // N+1 correction
    if (N > inlets_.size()) {
        OBJ_ERR << "invalid inlet index: " << sel->s_name;
        return false;
    }

    onInlet(N, lv);
    return true;
}

bool BaseObject::processAnyProps(t_symbol* sel, const AtomListView& lv)
{
    if (sel->s_name[0] != '@')
        return false;

    t_symbol* get_key = tryGetPropKey(sel);

    if (get_key) {
        // no outlets
        if (numOutlets() < 1)
            return true;

        // single property request
        if (lv.empty()) {
            AtomList res;
            if (!queryProperty(get_key, res))
                return false;

            anyTo(0, res);
        } else {
            // multiple property request
            AtomList res;
            queryProperty(get_key, res);

            for (auto& pname : lv) {
                t_symbol* s;
                if (!pname.getSymbol(&s))
                    continue;

                t_symbol* k = tryGetPropKey(s);
                if (!k)
                    continue;

                queryProperty(k, res);
            }

            if (res.empty())
                return false;

            anyTo(0, res);
        }
    } else {
        auto p = property(sel);
        if (!p) {
            OBJ_ERR << "invalid property: " << sel;
            return false;
        }

        bool rc = false;

        // support for string for property
        if (p->isSymbol() && lv.isA<DataTypeString>()) {
            auto str = lv.asD<DataTypeString>();
            const Atom sym(gensym(str->str().c_str()));
            rc = p->set(AtomListView(sym));
        } else
            rc = p->set(lv);

        if (!rc)
            OBJ_ERR << "can't set property: " << sel;

        return true;
    }

    return true;
}

void BaseObject::freeProps()
{
    for (auto p : props_)
        delete p;

    props_.clear();
}

void BaseObject::appendInlet(t_inlet* in)
{
    inlets_.push_back(in);
}

void BaseObject::appendOutlet(t_outlet* out)
{
    outlets_.push_back(out);
}

bool BaseObject::queryProperty(t_symbol* key, AtomList& res) const
{
    auto p = property(key);
    if (!p) {
        OBJ_ERR << "invalid property: " << key;
        return false;
    }

    res.append(key);
    res.append(p->get());
    return true;
}

bool BaseObject::popInlet()
{
    if (inlets_.empty())
        return false;

    auto x = inlets_.back();
    inlets_.pop_back();

    if (canvas() && owner())
        canvas_deletelinesforio(canvas(), owner(), x, reinterpret_cast<t_outlet*>(x));

    inlet_free(x);
    return true;
}

bool BaseObject::popOutlet()
{
    if (outlets_.empty())
        return false;

    auto x = outlets_.back();
    outlets_.pop_back();

    if (canvas() && owner())
        canvas_deletelinesforio(canvas(), owner(), reinterpret_cast<t_inlet*>(x), x);

    outlet_free(x);
    return true;
}

void BaseObject::initInletDispatchNames()
{
    char buf[5];
    buf[0] = '_';
    buf[1] = ':';
    buf[4] = '\0';

    for (size_t i = 0; i < inlet_dispatch_names.size(); i++) {
        int2hex(i + 1, &buf[2]); // inlet index in ascii-hex
        inlet_dispatch_names[i] = gensym(buf);
    }
}

t_outlet* BaseObject::createOutlet()
{
    t_outlet* out = outlet_new(pd_.owner, &s_list);
    outlets_.push_back(out);
    return out;
}

t_inlet* BaseObject::createInlet(t_float* v)
{
    inlets_.push_back(floatinlet_new(pd_.owner, v));
    return inlets_.back();
}

t_inlet* BaseObject::createInlet(t_symbol** s)
{
    inlets_.push_back(symbolinlet_new(pd_.owner, s));
    return inlets_.back();
}

size_t BaseObject::numInlets() const
{
    return pd_.owner ? static_cast<size_t>(obj_ninlets(pd_.owner)) : 0;
}

void BaseObject::freeInlets()
{
    for (auto x : inlets_)
        inlet_free(x);

    inlets_.clear();
}

void BaseObject::clearInlets()
{
    auto cnv = canvas();
    auto obj = owner();

    if (cnv && obj) {
        int i = 0;
        for (auto* x : inlets_) {
            post("i: %d", i++);
            //            canvas_deletelinesforio(cnv, obj, x, nullptr);
        }
    }
}

const char* BaseObject::annotateInlet(size_t n) const
{
    auto it = inlet_info_map.find(classPointer());
    if (it == inlet_info_map.end())
        return nullptr;

    if (n >= it->second.size())
        return nullptr;

    if (it->second[n].empty())
        return nullptr;

    return it->second[n].c_str();
}

BaseObject::BaseObject(const PdArgs& args)
    : pd_(args)
    , receive_from_(nullptr)
    , cnv_(canvas_getcurrent())
{
    if (pd_.parseArgs)
        parsePosArgs(pd_.parseArgsMode);

    Deprecated::instance().checkAlias(args.className, args.creationName);
}

BaseObject::~BaseObject()
{
    unbindReceive();
    freeInlets();
    freeOutlets();
    freeProps();
}

size_t BaseObject::positionalConstantP(size_t pos, size_t def, size_t min, size_t max) const
{
    if (pos >= pos_args_parsed_.size())
        return def;

    auto& arg = pos_args_parsed_[pos];
    if (!arg.isFloat()) {
        OBJ_ERR << "integer value >=0 expected at position: " << pos << ", using default value: " << def;
        return def;
    } else {
        if (!arg.isInteger())
            OBJ_ERR << "integer value expected at position: " << pos << ", rounding to: " << arg.asInt();

        int v = arg.asInt();
        if (v < 0 || v < min || v > max) {
            OBJ_ERR << "invalid value " << v << " at position: " << pos
                    << ", should be in [" << min
                    << "..." << max << "]"
                    << ", using: " << clip<long>(v, min, max);

            return static_cast<size_t>(clip<long>(v, min, max));
        } else
            return static_cast<size_t>(v);
    }
}

bool BaseObject::parsePosProperty(Property* p)
{
    const size_t NARGS = pos_args_parsed_.size();

    const size_t ARG_IDX = p->argIndex();
    if (p->hasArgIndex() && ARG_IDX < NARGS) {
        bool ok = false;

        if (p->isList())
            ok = p->setInit(pos_args_parsed_.view(ARG_IDX));
        else //  single atom
            ok = p->setInit(pos_args_parsed_.view(ARG_IDX, 1));

        if (!ok)
            OBJ_ERR << "can't set property: " << p->name()->s_name;

        return ok;
    }

    return false;
}

bool BaseObject::parseProperty(Property* p, const AtomListView& props, PdArgs::ParseMode mode)
{
    if (mode == PdArgs::PARSE_NONE)
        return false;

    const auto name = p->name();
    const auto NPROPS = props.size();

    for (size_t i = 0; i < NPROPS; i++) {
        const Atom& a = props[i];

        if (a == name) {
            size_t nargs = 0; // number of property arguments

            // lookup till next property
            for (size_t j = i + 1; j < NPROPS; j++, nargs++) {
                // next property found
                if (props[j].isProperty())
                    break;
            }

            if (nargs > 0) {
                auto v = props.subView(i + 1, nargs);
                // optimization: do not parse float, boolean or float list
                if (v.isFloat() || v.isBool() || v.allOf(isFloat)) {
                    if (!p->setInit(v)) {
                        OBJ_ERR << "can't set property: " << name->s_name;
                        return false;
                    } else
                        return true;
                } else { // have to parse
                    switch (mode) {
                    case PdArgs::PARSE_EXPR: {
                        auto parse_result = parseDataList(v);
                        if (parse_result) { // parse ok -> init property
                            if (!p->setInit(parse_result.result().view())) {
                                OBJ_ERR << "can't set property: " << name->s_name;
                                return false;
                            } else
                                return true;
                        } else { // parse error
                            OBJ_ERR << parse_result.err() << v;
                            return true;
                        }
                    } break;
                    case PdArgs::PARSE_UNQUOTE: {
                        if (string::maybe_ceammc_quoted_string(v)) {
                            auto unquoted = string::parse_ceammc_quoted_string(v);
                            if (!p->setInit(unquoted.view())) {
                                OBJ_ERR << fmt::format("can't set property: '{}'", name->s_name);
                                return false;
                            } else
                                return true;
                        } else { // no quoted atoms
                            if (!p->setInit(v)) {
                                OBJ_ERR << fmt::format("can't set property: '{}'", name->s_name);
                                return false;
                            } else
                                return true;
                        }
                    } break;
                    default:

                        if (!p->setInit(v)) {
                            OBJ_ERR << "can't set property: " << name->s_name;
                            return false;
                        } else
                            return true;

                        break;
                    }
                }
            } else { // empty args, flags or lists, for example
                // init property
                if (!p->setInit({})) {
                    OBJ_ERR << "can't set property: " << name->s_name;
                    return false;
                } else
                    return true;
            }
        } else if (a.isProperty() && !hasProperty(a.asT<t_symbol*>())) {
            OBJ_ERR << "unknown property in argument list: " << a;
        }
    }

    return false;
}

void BaseObject::parseProps(int flags, PdArgs::ParseMode mode)
{
    if (mode == PdArgs::PARSE_NONE)
        return;

    const auto first_prop = pd_.args.findPos(isProperty);
    AtomListView prop_args;
    if (first_prop >= 0)
        prop_args = pd_.args.view(first_prop);

    for (Property* p : props_) {
        // skip non-public properties
        if (p->isReadOnly() || p->isInternal())
            continue;

        int init_times = 0;

        // process positional args
        if ((flags & PROP_PARSE_POS) && parsePosProperty(p))
            init_times++;

        // process normal properties
        if (prop_args.size() > 0 && (flags & PROP_PARSE) && parseProperty(p, prop_args, mode))
            init_times++;

        if (init_times > 1) {
            OBJ_POST << fmt::format("property '{0}' was set twice: from positional ({1:d}) and property ({0}) arguments",
                p->name()->s_name,
                p->argIndex());
        }
    }
}

AtomListView BaseObject::binbufArgs() const
{
    if (!owner() || !owner()->te_binbuf) { // no binbuf: object created not from GUI
        return pd_.args.view(0); // for test purposes we are returning creation args
    }

    return AtomListView(binbuf_getvec(owner()->te_binbuf), binbuf_getnatom(owner()->te_binbuf)).subView(1);
}

void BaseObject::parseProperties()
{
    int flags = 0;
    if (pd_.parseProps)
        flags |= PROP_PARSE;

    if (pd_.parsePosProps)
        flags |= PROP_PARSE_POS;

    if (flags)
        parseProps(flags, pd_.parsePropsMode);
}

void BaseObject::parsePosArgs(PdArgs::ParseMode mode)
{
    if (mode == PdArgs::PARSE_NONE)
        return;

    auto PROP_START = pd_.args.findPos(isProperty);
    if (PROP_START == 0) // no positional args
        return;

    const auto args = pd_.args.view(0, PROP_START);

    switch (mode) {
    case PdArgs::PARSE_COPY:
        pos_args_parsed_ = args;
        break;
    case PdArgs::PARSE_UNQUOTE: {
        if (string::maybe_ceammc_quoted_string(args))
            pos_args_parsed_ = string::parse_ceammc_quoted_string(args);
        else
            pos_args_parsed_ = args;
    } break;
    case PdArgs::PARSE_EXPR: {
        auto parse_result = parseDataList(args);
        if (parse_result) { // parse ok
            pos_args_parsed_ = parse_result.result();
        } else {
            OBJ_ERR << parse_result.err();

            // save as is
            pos_args_parsed_ = args;
        }
    } break;
    case PdArgs::PARSE_NONE:
    default:
        return;
    }
}

void BaseObject::updatePropertyDefaults()
{
    for (auto p : props_)
        p->updateDefault();
}

void BaseObject::initDone() { }

bool BaseObject::checkArg(const Atom& atom, BaseObject::ArgumentType type, int pos) const
{
#define ARG_ERROR(msg)                    \
    {                                     \
        std::ostringstream os;            \
        os << "invalid argument";         \
        if (pos >= 0)                     \
            os << " at position " << pos; \
        os << ": " << msg;                \
        OBJ_ERR << os.str();              \
        return false;                     \
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
        if (!(atom.isInteger() && atom >= 0))
            ARG_ERROR("natural expected");
        break;
    case ARG_BOOL:
        if (!atom.isFloat())
            ARG_ERROR("boolean expected");

        if (atom.asFloat() != 0.f && atom.asFloat() != 1.f)
            ARG_ERROR("only 1 or 0 accepted");

        break;
    case ARG_BYTE:
        if (!atom.isFloat())
            ARG_ERROR("byte value expected");

        if (atom.asFloat() < 0 || atom.asFloat() > 255)
            ARG_ERROR("byte range expected: [0-255]");

        break;
    }

    return true;

#undef ARG_ERROR
}

static const char* to_string(BaseObject::ArgumentType a)
{
    static const char* names[] = {
        "ARG_FLOAT",
        "ARG_INT",
        "ARG_NATURAL",
        "ARG_SYMBOL",
        "ARG_PROPERTY",
        "ARG_NON-PROPERTY_SYMBOL",
        "ARG_BOOL",
        "ARG_BYTE"
    };

    if (a >= boost::size(names))
        return "??? fixme";

    return names[a];
}

bool BaseObject::checkArgs(const AtomListView& lv, ArgumentType a1, t_symbol* method) const
{
    if (lv.size() < 1
        || !checkArg(lv[0], a1, 0)) {

        if (method)
            OBJ_ERR << "Usage: " << method->s_name << " " << to_string(a1);

        return false;
    }

    return true;
}

bool BaseObject::checkArgs(const AtomListView& lv, BaseObject::ArgumentType a1,
    BaseObject::ArgumentType a2, t_symbol* method) const
{
    if (lv.size() < 2
        || !checkArg(lv[0], a1, 0)
        || !checkArg(lv[1], a2, 1)) {

        if (method)
            OBJ_ERR << "Usage: " << method->s_name
                    << " " << to_string(a1)
                    << " " << to_string(a2);

        return false;
    }

    return true;
}

bool BaseObject::checkArgs(const AtomListView& lv, BaseObject::ArgumentType a1,
    BaseObject::ArgumentType a2, BaseObject::ArgumentType a3, t_symbol* method) const
{
    if (lv.size() < 3
        || !checkArg(lv[0], a1, 0)
        || !checkArg(lv[1], a2, 1)
        || !checkArg(lv[2], a3, 2)) {

        if (method)
            OBJ_ERR << "Usage: " << method->s_name
                    << " " << to_string(a1)
                    << " " << to_string(a2)
                    << " " << to_string(a3);

        return false;
    }

    return true;
}

bool BaseObject::checkArgs(const AtomListView& lv, BaseObject::ArgumentType a1,
    BaseObject::ArgumentType a2, BaseObject::ArgumentType a3,
    BaseObject::ArgumentType a4, t_symbol* method) const
{

    if (lv.size() < 4
        || !checkArg(lv[0], a1, 0)
        || !checkArg(lv[1], a2, 1)
        || !checkArg(lv[2], a3, 2)
        || !checkArg(lv[3], a4, 3)) {

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
    auto& dict = ObjectInfoStorage::instance().info(classPointer()).dict;
    auto it = dict.find("description");
    if (it != dict.end() && !it->second.empty())
        post("**%s**", it->second.c_str());

    // cast from size_t -> int; for all supported OS-platform to be happy
    post("[%s] inlets: %i", className()->s_name, static_cast<int>(numInlets()));
    post("[%s] outlets: %i", className()->s_name, static_cast<int>(numOutlets()));

    for (auto p : props_) {
        post("[%s] property: %s = %s",
            className()->s_name,
            p->name()->s_name,
            to_string(p->get()).c_str());
    }
}

bool BaseObject::isVisible() const
{
    return owner()
        && canvas()
        && owner()->te_binbuf
        && glist_isvisible(const_cast<t_glist*>(canvas()))
        && gobj_shouldvis((t_gobj*)owner(), const_cast<t_glist*>(canvas()));
}

void BaseObject::show(bool value)
{
    if (owner() && canvas())
        gobj_vis((t_gobj*)owner(), canvas(), value ? 1 : 0);
}

void BaseObject::fixLines()
{
    if (owner() && canvas())
        canvas_fixlinesfor(canvas(), owner());
}

void BaseObject::queryPropNames()
{
    AtomList res;
    res.reserve(props_.size());

    for (auto p : props_)
        res.append(Atom(p->name()));

    if (outlets_.empty()) {
        // dump to console
        OBJ_DBG << res;
    } else
        outletAny(outlets_.front(), SYM_PROPS_ALL(), res);
}

void BaseObject::onBang()
{
    OBJ_ERR << "bang is not expected";
}

void BaseObject::onFloat(t_float)
{
    OBJ_ERR << "float is not expected";
}

void BaseObject::onSymbol(t_symbol*)
{
    OBJ_ERR << "symbol is not expected";
}

void BaseObject::onList(const AtomListView&)
{
    OBJ_ERR << "list is not expected";
}

void BaseObject::onData(const Atom& d)
{
    OBJ_ERR << "data is not expected: " << d.asData()->typeName();
}

void BaseObject::onAny(t_symbol* s, const AtomListView&)
{
    OBJ_ERR << "unexpected message: " << s;
}

void BaseObject::onClick(t_floatarg /*xpos*/, t_floatarg /*ypos*/,
    t_floatarg /*shift*/, t_floatarg /*ctrl*/, t_floatarg /*alt*/)
{
    OBJ_ERR << "not implemeneted";
}

void BaseObject::dispatchLoadBang(int action)
{
    switch (action) {
    case LB_LOAD:
        onLoadBang();
        break;
    case LB_INIT:
        onInitBang();
        break;
    case LB_CLOSE:
        onCloseBang();
        break;
    default:
        OBJ_ERR << "unknown loadbang type: " << action;
        break;
    }
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
        if (receive_from_->s_thing)
            pd_unbind(&owner()->te_g.g_pd, receive_from_);

        receive_from_ = 0;
    }
}

t_symbol* BaseObject::receive()
{
    return receive_from_;
}

t_canvas* BaseObject::rootCanvas()
{
    if (!cnv_)
        return nullptr;

    return canvas_getrootfor(cnv_);
}

t_canvas* BaseObject::rootCanvas() const
{
    if (!cnv_)
        return nullptr;

    return canvas_getrootfor(const_cast<t_canvas*>(cnv_));
}

bool BaseObject::isPatchLoading() const
{
    auto* c = canvas();
    return c ? c->gl_loading : false;
}

std::string BaseObject::findInStdPaths(const char* fname) const
{
    return platform::find_in_std_path(cnv_, fname);
}

t_symbol* BaseObject::tryGetPropKey(t_symbol* sel)
{
    if (!sel)
        return nullptr;

    const char* str = sel->s_name;
    if (!str || !str[0])
        return nullptr;

    const size_t last_idx = strlen(str) - 1;

    if (last_idx >= MAXPDSTRING)
        return nullptr;

    if (str[last_idx] == '?') {
        char buf[MAXPDSTRING] = { 0 };
        memcpy(&buf, str, last_idx);
        buf[last_idx] = '\0';
        return gensym(buf);
    }

    return nullptr;
}

bool BaseObject::isAbsolutePath(const char* path)
{
    return sys_isabsolutepath(path) == 1;
}

void BaseObject::addInletInfo(t_class* c, const std::string& txt)
{
    inlet_info_map[c].push_back(txt);
}

void BaseObject::addOutletInfo(_class* c, const std::string& txt)
{
    outlet_info_map[c].push_back(txt);
}

void BaseObject::setInletsInfo(_class* c, const BaseObject::XletInfo& l)
{
    inlet_info_map[c] = l;
}

void BaseObject::setOutletsInfo(_class* c, const BaseObject::XletInfo& l)
{
    outlet_info_map[c] = l;
}
}
