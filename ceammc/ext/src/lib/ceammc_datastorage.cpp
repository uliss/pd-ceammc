/*****************************************************************************
 * Copyright 2017 Serge Poltavsky. All rights reserved.
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
#include "ceammc_datastorage.h"
#include "ceammc_atomlist.h"
#include "ceammc_convert.h"
#include "ceammc_datatypes.h"
#include "ceammc_format.h"
#include "ceammc_log.h"
#include "fmt/format.h"
#include "muparser/muparser/include/muParser.h"

#include <algorithm>
#include <cmath>

namespace ceammc {

DataStorage& DataStorage::instance()
{
    static DataStorage s;
    return s;
}

int DataStorage::registerNewType(const std::string& name,
    CreateFromListFn fromListFn,
    CreateFromDictFn fromDictFn)
{
    constexpr auto FIRST_TYPE_ID = 256;

    if (type_list_.empty()) {
        type_list_.emplace_back(FIRST_TYPE_ID, name, fromListFn, fromDictFn);
        return FIRST_TYPE_ID;
    } else {
        const auto LAST_ID = type_list_.back().type;

        if (LAST_ID >= type_list_.capacity()) {
            LIB_ERR << fmt::format(
                "can't register type {}, max number of types ({}) is exceed: {}",
                name, type_list_.capacity(), __FUNCTION__);
            return data::DATA_INVALID;
        }

        if (type_list_.capacity() == type_list_.size()) {
            LIB_ERR << fmt::format("can't register type {}, datastorage overflow: {}", name, __FUNCTION__);
            return data::DATA_INVALID;
        }

        if (findByName(name) != type_list_.cend()) {
            LIB_ERR << fmt::format("can't register type {}, it already exists: {}", name, __FUNCTION__);
            return data::DATA_INVALID;
        }

        const auto NEW_ID = LAST_ID + 1;
        LIB_LOG << fmt::format("new data type {} is registered with id: {}", name, NEW_ID);

        type_list_.emplace_back(NEW_ID, name, fromListFn, fromDictFn);
        return NEW_ID;
    }
}

int DataStorage::typeByName(const std::string& name) const
{
    auto it = findByName(name);
    return (it == type_list_.cend()) ? data::DATA_INVALID
                                     : it->type;
}

std::string DataStorage::nameByType(int type) const
{
    auto it = findByType(type);
    return (it == type_list_.cend()) ? std::string()
                                     : it->name;
}

CreateFromListFn DataStorage::fromListFunction(const std::string& name) const
{
    auto it = std::find_if(type_list_.begin(), type_list_.end(),
        [&name](const DataTypeRecord& r) { return r.name == name; });

    return (it == type_list_.end()) ? nullptr : it->from_list_fn;
}

CreateFromDictFn DataStorage::fromDictFunction(const std::string& name) const
{
    auto it = std::find_if(type_list_.begin(), type_list_.end(),
        [&name](const DataTypeRecord& r) { return r.name == name; });

    return (it == type_list_.end()) ? nullptr : it->from_dict_fn;
}

void DataStorage::clearAll()
{
    type_list_.clear();
}

double muParse(const std::string& expr)
{
    using DD = double (*)(double);
    mu::Parser p;
    static const t_float m_pi = std::acos(t_float(-1));
    static const t_float m_exp = std::exp(t_float(1));

    p.DefineNameChars("$0123456789_abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ");
    p.DefineConst("sr", sys_getsr());
    p.DefineConst("bs", sys_getblksize());
    p.DefineConst("e", m_exp);
    p.DefineConst("pi", m_pi);
    p.DefineFun("floor", (DD)std::floor);
    p.DefineFun("ceil", (DD)std::ceil);
    p.SetExpr(expr);
    return p.Eval();
}

DataStorage::DataStorage()
{
    static const t_float m_pi = std::acos(t_float(-1));
    static const t_float m_exp = std::exp(t_float(1));
    registerNewType("pi", [](const AtomList&) -> Atom { return Atom(m_pi); });
    registerNewType("e", [](const AtomList&) -> Atom { return Atom(m_exp); });
    registerNewType("sr", [](const AtomList&) -> Atom { return Atom(sys_getsr()); });
    registerNewType("bs", [](const AtomList&) -> Atom { return Atom(sys_getblksize()); });
    registerNewType("mtof", [](const AtomList& a) -> Atom { return Atom(convert::midi2freq(a.floatAt(0, 0))); });
    registerNewType("ftom", [](const AtomList& a) -> Atom { return Atom(convert::freq2midi(a.floatAt(0, 0))); });
    registerNewType("ms2bpm", [](const AtomList& a) -> Atom { return Atom(60000 / a.floatAt(0, 1)); });
    registerNewType("bpm2ms", [](const AtomList& a) -> Atom { return Atom(60000 / a.floatAt(0, 1)); });

    registerNewType("expr", [](const AtomList& expr) -> Atom {
        std::string str = to_string(expr);
        try {
            return Atom(muParse(str));
        } catch (mu::Parser::exception_type& e) {
            pd_error(nullptr, "[muparser] exception: %s", e.GetMsg().c_str());
        } catch (std::exception& e) {
            pd_error(nullptr, "[muparser] exception: %s", e.what());
        }

        return Atom(0.f);
    });
}

DataStorage::type_iterator DataStorage::findByName(const std::string& name) const
{
    return std::find_if(
        type_list_.begin(),
        type_list_.end(),
        [name](const DataTypeRecord& r) { return r.name == name; });
}

DataStorage::type_iterator DataStorage::findByType(int type) const
{
    return std::find_if(
        type_list_.begin(),
        type_list_.end(),
        [type](const DataTypeRecord& r) { return r.type == type; });
}

}
