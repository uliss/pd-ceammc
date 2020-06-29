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
#include "ceammc_datatypes.h"
#include "ceammc_format.h"
#include "ceammc_log.h"
#include "fmt/format.h"
#include "lex/exprtk.hpp"

#include <algorithm>

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

DataStorage::DataStorage()
{
    registerNewType("pi", [](const AtomList&) -> Atom { return Atom(3.14159265358979); });
    registerNewType("e", [](const AtomList&) -> Atom { return Atom(2.71828182846); });
    registerNewType("sr", [](const AtomList&) -> Atom { return Atom(sys_getsr()); });
    registerNewType("bs", [](const AtomList&) -> Atom { return Atom(sys_getblksize()); });
    registerNewType("expr", [](const AtomList& expr) -> Atom {
        using symbol_table_t = exprtk::symbol_table<t_float>;
        using expression_t = exprtk::expression<t_float>;
        using parser_t = exprtk::parser<t_float>;

        const std::string expression_string = to_string(expr);

        symbol_table_t symbol_table;
        symbol_table.add_constants();
        symbol_table.add_constant("e", std::exp(1));
        symbol_table.add_constant("sr", sys_getsr());
        symbol_table.add_constant("bs", sys_getblksize());


        expression_t expression;
        expression.register_symbol_table(symbol_table);

        parser_t parser;
        parser.compile(expression_string, expression);

        return Atom(expression.value());
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
