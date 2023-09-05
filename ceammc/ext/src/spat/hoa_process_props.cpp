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
#include "ceammc_containers.h"
#include "ceammc_crc32.h"
#include "ceammc_factory.h"
#include "ceammc_fn_list.h"
#include "ceammc_format.h"
#include "datatype_property.h"
#include "fmt/core.h"
#include "hoa_process.h"

#include <stdexcept>

CEAMMC_DEFINE_SYM_HASH(2d)
CEAMMC_DEFINE_SYM_HASH(3d)
CEAMMC_DEFINE_SYM_HASH(harmonics)
CEAMMC_DEFINE_SYM_HASH(planewaves)

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
        ->setSymbolEnumCheck({ sym_2d(), sym_3d() });

    createCbSymbolProperty("@ptype", [this]() -> t_symbol* { return args_.type; })
        ->setSymbolEnumCheck({ sym_harmonics(), sym_planewaves() });

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

bool HoaProcessProps::eachProperty(const AtomListView& lv,
    std::function<bool(t_symbol*)> is_valid,
    std::function<void(Property*, t_symbol*, const AtomListView&)> inner_process,
    std::function<void(DataTypeProperty*, const std::string&, const AtomListView&)> declared_process)
{
    std::vector<t_symbol*> all_declared_keys;
    PropertyStorage::storage().keys(all_declared_keys);
    int cnt = 0; // success count

    // split list to list of properties
    auto prop = list::findProperty(lv);
    while (!prop.first.empty()) {
        const auto& args = prop.first;
        assert(args[0].isProperty());

        t_symbol* p = args[0].asSymbol();

        // may skip some properties
        if (!is_valid(p))
            continue;

        // search inner property
        for (auto inner_prop : properties()) {
            // found inner property
            if (prop_equal(inner_prop->name()->s_name, p->s_name)) {
                inner_process(inner_prop, inner_prop->name(), args.subView(1));
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
                    continue; // inner continue

                // key found
                auto prop = PropertyStorage::storage().acquire(k);
                if (!prop) // but not acquired
                    break; // break inner loop, check other prop request

                declared_process(prop, prop_name, args.subView(1));

                PropertyStorage::storage().release(k);
                cnt++;
                goto continue_label;
            }

            OBJ_ERR << "property is not found: " << prop_name;
        }

    continue_label:

        prop = list::findProperty(prop.second);
        continue;
    }

    return cnt > 0;
}

bool HoaProcessProps::processAnyProps(t_symbol* sel, const AtomListView& lv)
{
    if (sel->s_name[0] != '@')
        return false;

    SmallAtomList res { sel };
    res.insert(res.end(), lv.begin(), lv.end());

    if (isPropQuery(sel)) {
        AtomList out;

        eachProperty(
            res.view(),
            [this](t_symbol* s) {
                if (!isPropQuery(s)) {
                    OBJ_ERR << "property query expected: " << s->s_name;
                    return false;
                } else
                    return true;
            },
            [&out](Property* prop, t_symbol* name, const AtomListView&) {
                out.append(name);
                out.append(prop->get());
            },
            [&out](DataTypeProperty* prop, const std::string& name, const AtomListView&) {
                out.append(gensym(name.c_str()));
                propToList(prop, out);
            });

        anyTo(0, out);
    } else {
        eachProperty(
            res.view(),
            [this](t_symbol* s) {
                bool ok = Atom(s).isProperty() && !isPropQuery(s);
                if (!ok) {
                    OBJ_ERR << "set property expected: " << s;
                    return false;
                } else
                    return true;
            },
            [](Property*, t_symbol*, const AtomListView&) {},
            [this](DataTypeProperty* prop, const std::string&, const AtomListView& args) {
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
                            prop->setFromPdArgs(args.subView(idx, 1));
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

void HoaProcessProps::onList(const AtomListView& lv)
{
    if (lv.size() != args_.total) {
        OBJ_ERR << "invalid list size: " << lv.size() << ", expected: " << args_.total;
        return;
    }

    auto idx = args_.index;
    if (idx >= lv.size()) {
        OBJ_ERR << "invalid index: " << idx;
        return;
    }

    atomTo(0, lv[idx]);
}

HoaProcessPropsData processHoaProps(const AtomListView& lv)
{
    if (lv.size() < 5) {
        std::ostringstream os;
        os << "at least 5 arguments required: " << lv;
        throw std::runtime_error(os.str());
    }

    auto mode = lv[0].asSymbol();
    auto type = lv[1].asSymbol();
    auto order = lv[2];
    auto idx0 = lv[3];
    auto idx1 = lv[4];

    const auto mode_hash = crc32_hash(mode);
    const auto type_hash = crc32_hash(type);

    if (mode_hash != hash_2d && mode_hash != hash_3d)
        throw std::runtime_error(fmt::format("invalid mode: '{}', expecting '{}' or '{}'", mode->s_name, str_2d, str_3d));

    if (type_hash != hash_harmonics && type_hash != hash_planewaves)
        throw std::runtime_error(fmt::format("invalid type: '{}', expecting '{}' or '{}'", type->s_name, str_harmonics, str_planewaves));

    if (!order.isFloat())
        throw std::runtime_error(fmt::format("order value expected: {}", to_string(order)));

    if (order.asFloat() < 1)
        throw std::runtime_error(fmt::format("order value should be >= 1, got {}", to_string(order)));

    if (!idx0.isFloat() || !idx1.isFloat())
        throw std::runtime_error(fmt::format("integer values expected: {} {}", to_string(idx0), to_string(idx1)));

    HoaProcessPropsData res;
    res.mode = mode;
    res.type = type;
    res.order = order.asInt();

    // total
    if (mode_hash == hash_2d && type_hash == hash_harmonics) {
        res.total = HoaProcess::calcNumHarm2d(res.order);
    } else if (mode_hash == hash_3d && type_hash == hash_harmonics) {
        res.total = HoaProcess::calcNumHarm3d(res.order);
    } else {
        res.total = res.order;
    }

    // index
    if (mode_hash == hash_2d && type_hash == hash_harmonics) {
        res.index = HoaProcess::harmToIndex2d(idx0.asInt(), idx1.asInt());
    } else if (mode_hash == hash_3d && type_hash == hash_harmonics) {
        res.index = HoaProcess::harmToIndex3d(idx0.asInt(), idx1.asInt());
    } else {
        res.index = idx0.asInt();
    }

    // harm degree
    if (type_hash == hash_harmonics)
        res.harm_degree = idx0.asInt();
    else
        res.harm_degree = -1;

    // harm order
    if (type_hash == hash_harmonics)
        res.harm_order = idx1.asInt();
    else
        res.harm_order = -1;

    return res;
}

void setup_spat_hoa_process_props()
{
    ObjectFactory<HoaProcessProps> obj("hoa.@process");
}
