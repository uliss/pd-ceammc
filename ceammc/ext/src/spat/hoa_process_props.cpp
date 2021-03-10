/*****************************************************************************
 * Copyright 2019 Serge Poltavsky. All rights reserved.
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
#include "hoa_process_props.h"
#include "ceammc_canvas.h"
#include "ceammc_factory.h"
#include "ceammc_format.h"
#include "datatype_property.h"
#include "hoa_process.h"

#include <sstream>
#include <stdexcept>

static t_symbol* SYM_2D;
static t_symbol* SYM_3D;
static t_symbol* SYM_HARMONICS;
static t_symbol* SYM_PLANEWAVES;

HoaProcessProps::HoaProcessProps(const PdArgs& args)
    : BaseObject(args)
    , args_ { &s_, &s_, 0, 0, 0, 0, 0 }
{
    // init prop_canvas_id_
    char buf[30];
    snprintf(buf, sizeof(buf), "%p:", (void*)canvas());
    prop_canvas_id_ = buf;

    createOutlet();

    createCbSymbolProperty("@pmode", [this]() -> t_symbol* { return args_.mode; })
        ->setSymbolEnumCheck({ SYM_2D, SYM_3D });

    createCbSymbolProperty("@ptype", [this]() -> t_symbol* { return args_.type; })
        ->setSymbolEnumCheck({ SYM_HARMONICS, SYM_PLANEWAVES });

    createCbIntProperty("@order", [this]() -> int { return args_.order; });
    createCbIntProperty("@total", [this]() -> int { return args_.total; });
    createCbIntProperty("@index", [this]() -> int { return args_.index; });
    createCbIntProperty("@hdegree", [this]() -> int { return args_.harm_degree; });
    createCbIntProperty("@horder", [this]() -> int { return args_.harm_order; });

    auto cnv_args = canvas_info_args(canvas());
    if (!cnv_args.empty())
        args_ = processHoaProps(cnv_args);
}

static void propToList(DataTypeProperty* prop, AtomList& out)
{
    switch (prop->propertyType()) {
    case PropValueType::BOOLEAN: {
        bool b = false;
        if (prop->getBool(b))
            out.append(Atom(b ? 1.f : 0.f));
    } break;
    case PropValueType::FLOAT: {
        t_float f = 0;
        if (prop->getFloat(f))
            out.append(Atom(f));
    } break;
    case PropValueType::INTEGER: {
        int i = 0;
        if (prop->getInt(i))
            out.append(Atom(i));
    } break;
    case PropValueType::SYMBOL: {
        t_symbol* s = &s_;
        if (prop->getSymbol(&s))
            out.append(Atom(s));
    } break;
    case PropValueType::LIST: {
        AtomList l;
        if (prop->getList(l))
            out.append(l);
    } break;
    default:
        break;
    }
}

static bool isPropQuery(t_symbol* s)
{
    if (s->s_name[0] != '@')
        return false;
    else
        return s->s_name[strlen(s->s_name) - 1] == '?';
}

static std::string canonicPropName(t_symbol* s)
{
    const size_t N = strlen(s->s_name);

    if (s->s_name[N - 1] != '?')
        return s->s_name;
    else
        return std::string(s->s_name, N - 1);
}

// calc @prop == @prop?
static bool prop_equal(const char* a, const char* b)
{
    while (!((*a == '\0' && *b == '?')
        || (*a == '?' && *b == '\0')
        || (*a == '\0' && *b == '\0'))) {

        if (*a != *b)
            return false;

        ++a;
        ++b;
    }

    return true;
}

bool HoaProcessProps::eachProperty(const AtomList& lst,
    std::function<bool(t_symbol*)> is_valid,
    std::function<void(Property*, t_symbol*, const AtomList&)> inner_process,
    std::function<void(DataTypeProperty*, const std::string&, const AtomList&)> declared_process)
{
    std::vector<t_symbol*> all_declared_keys;
    PropertyStorage::storage().keys(all_declared_keys);
    int cnt = 0; // success count

    // split list to list of properties
    for (auto& args : lst.properties()) {
        assert(args[0].isProperty());

        t_symbol* p = args[0].asSymbol();

        // may skip some properties
        if (!is_valid(p))
            continue;

        // search inner property
        for (auto inner_prop : properties()) {
            // found inner property
            if (prop_equal(inner_prop->name()->s_name, p->s_name)) {
                inner_process(inner_prop, inner_prop->name(), args.slice(1));
                cnt++;
                goto continue_label;
            }
        }

        // search declared property
        { // @prop? -> @prop
            const std::string prop_name = canonicPropName(p);

            // @prop -> @0xDEADBEEF:@prop (inner property name)
            const std::string full_name = prop_canvas_id_ + prop_name;

            // not using gensym for guessing key name
            for (auto k : all_declared_keys) {
                // using slower O(n) string compare instead of quick t_symbol* compare O(1)
                // to prevent increasing t_symbol table
                if (full_name != k->s_name)
                    continue;

                // key found
                auto prop = PropertyStorage::storage().acquire(k);
                if (!prop) // but not acquired
                    break; // break inner loop, check other prop request

                declared_process(prop, prop_name, args.slice(1));

                PropertyStorage::storage().release(k);
                cnt++;
                goto continue_label;
            }

            OBJ_ERR << "property is not found: " << prop_name;
        }

    continue_label:
        continue;
    }

    return cnt > 0;
}

bool HoaProcessProps::processAnyProps(t_symbol* sel, const AtomListView& lst)
{
    if (sel->s_name[0] != '@')
        return false;

    if (isPropQuery(sel)) {
        AtomList out;

        eachProperty(
            AtomList(sel) + lst,
            [this](t_symbol* s) {
                if (!isPropQuery(s)) {
                    OBJ_ERR << "property query expected: " << s->s_name;
                    return false;
                } else
                    return true;
            },
            [&out](Property* prop, t_symbol* name, const AtomList&) {
                out.append(name);
                out.append(prop->get());
            },
            [&out](DataTypeProperty* prop, const std::string& name, const AtomList&) {
                out.append(gensym(name.c_str()));
                propToList(prop, out);
            });

        anyTo(0, out);
    } else {
        eachProperty(
            AtomList(sel) + lst,
            [this](t_symbol* s) {
                bool ok = Atom(s).isProperty() && !isPropQuery(s);
                if (!ok) {
                    OBJ_ERR << "set property expected: " << s;
                    return false;
                } else
                    return true;
            },
            [](Property*, t_symbol*, const AtomList&) {},
            [this](DataTypeProperty* prop, const std::string&, const AtomList& args) {
                // per instance args
                if (args.size() > 1) {
                    switch (prop->propertyType()) {
                    case PropValueType::BOOLEAN:
                    case PropValueType::FLOAT:
                    case PropValueType::INTEGER:
                    case PropValueType::SYMBOL: {
                        size_t idx = this->args_.index;
                        if (idx >= args.size()) {
                            OBJ_ERR << "invalid list size: " << idx << ", index not exists: " << idx;
                        } else {
                            prop->setFromPdArgs(args.slice(idx, idx + 1));
                        }
                    } break;
                    case PropValueType::LIST:
                    default:
                        prop->setFromPdArgs(args);
                        break;
                    }
                } else {
                    prop->setFromPdArgs(args);
                }
            });
    }

    return true;
}

void HoaProcessProps::onBang()
{
    bangTo(0);
}

void HoaProcessProps::onFloat(t_float f)
{
    floatTo(0, f);
}

void HoaProcessProps::onSymbol(t_symbol* s)
{
    symbolTo(0, s);
}

void HoaProcessProps::onList(const AtomList& lst)
{
    if (lst.size() != args_.total) {
        OBJ_ERR << "invalid list size: " << lst.size() << ", expected: " << args_.total;
        return;
    }

    auto idx = args_.index;
    if (idx >= lst.size()) {
        OBJ_ERR << "invalid index: " << idx;
        return;
    }

    atomTo(0, lst[idx]);
}

HoaProcessPropsData processHoaProps(const AtomList& lst)
{
    if (lst.size() < 5) {
        std::ostringstream os;
        os << "at least 5 arguments required: " << lst;
        throw std::runtime_error(os.str());
    }

    auto mode = lst[0].asSymbol();
    auto type = lst[1].asSymbol();
    auto order = lst[2];
    auto idx0 = lst[3];
    auto idx1 = lst[4];

    if (mode != SYM_2D && mode != SYM_3D) {
        std::ostringstream os;
        os << "invalid mode: " << mode << ", expecting " << SYM_2D << " or " << SYM_3D;
        throw std::runtime_error(os.str());
    }

    if (type != SYM_HARMONICS && type != SYM_PLANEWAVES) {
        std::ostringstream os;
        os << "invalid type: " << type << ", expecting " << SYM_HARMONICS << " or " << SYM_PLANEWAVES;
        throw std::runtime_error(os.str());
    }

    if (!order.isFloat()) {
        std::ostringstream os;
        os << "order value expected: " << order;
        throw std::runtime_error(os.str());
    }

    if (order.asFloat() < 1) {
        std::ostringstream os;
        os << "order value should be >= 1: " << order;
        throw std::runtime_error(os.str());
    }

    if (!idx0.isFloat() || !idx1.isFloat()) {
        std::ostringstream os;
        os << "integer values expected: " << idx0 << " " << idx1;
        throw std::runtime_error(os.str());
    }

    HoaProcessPropsData res;
    res.mode = mode;
    res.type = type;
    res.order = order.asInt();

    // total
    if (mode == SYM_2D && type == SYM_HARMONICS) {
        res.total = HoaProcess::calcNumHarm2d(res.order);
    } else if (mode == SYM_3D && type == SYM_HARMONICS) {
        res.total = HoaProcess::calcNumHarm3d(res.order);
    } else {
        res.total = res.order;
    }

    // index
    if (mode == SYM_2D && type == SYM_HARMONICS) {
        res.index = HoaProcess::harmToIndex2d(idx0.asInt(), idx1.asInt());
    } else if (mode == SYM_3D && type == SYM_HARMONICS) {
        res.index = HoaProcess::harmToIndex3d(idx0.asInt(), idx1.asInt());
    } else {
        res.index = idx0.asInt();
    }

    // harm degree
    if (type == SYM_HARMONICS)
        res.harm_degree = idx0.asInt();
    else
        res.harm_degree = -1;

    // harm order
    if (type == SYM_HARMONICS)
        res.harm_order = idx1.asInt();
    else
        res.harm_order = -1;

    return res;
}

void setup_spat_hoa_process_props()
{
    SYM_2D = gensym("2d");
    SYM_3D = gensym("3d");
    SYM_HARMONICS = gensym("harmonics");
    SYM_PLANEWAVES = gensym("planewaves");

    ObjectFactory<HoaProcessProps> obj("hoa.@process");
}
