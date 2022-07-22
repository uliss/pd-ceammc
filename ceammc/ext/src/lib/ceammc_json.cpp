/*****************************************************************************
 * Copyright 2020 Serge Poltavsky. All rights reserved.
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
#include "ceammc_json.h"
#include "ceammc_format.h"
#include "datatype_dict.h"
#include "datatype_mlist.h"
#include "datatype_string.h"

#include "json/json.hpp"

namespace ceammc {
namespace json {

    static nlohmann::json to_json_struct(const Atom& a);
    static nlohmann::json to_json_struct(const AtomList& l);
    static nlohmann::json to_json_struct(const DataTypeDict& dict);
    static nlohmann::json to_json_struct(const DataTypeString& str);

    static nlohmann::json to_json_struct(const AtomList& l)
    {
        nlohmann::json arr = nlohmann::json::array();

        for (const Atom& a : l)
            arr.push_back(to_json_struct(a));

        return arr;
    }

    static nlohmann::json to_json_struct(const DataTypeDict& dict)
    {
        auto obj = nlohmann::json::object();

        for (auto& kv : dict) {
            auto key = kv.first->s_name;

            auto& value = kv.second;
            if (value.isA<Atom>() )
                obj[key] = to_json_struct(value.asT<Atom>());
            else
                obj[key] = to_json_struct(value);
        }

        return obj;
    }

    static nlohmann::json to_json_struct(const DataTypeString& str)
    {
        return nlohmann::json(str.str());
    }

    static nlohmann::json to_json_struct(const Atom& a)
    {
        if (a.isSymbol())
            return nlohmann::json(a.asSymbol()->s_name);
        else if (a.isInteger())
            return nlohmann::json(a.asInt());
        else if (a.isFloat())
            return nlohmann::json(a.asFloat());
        else if (a.isA<DataTypeMList>())
            return to_json_struct(a.asD<DataTypeMList>()->data());
        else if (a.isA<DataTypeString>())
            return to_json_struct(*a.asD<DataTypeString>());
        else if (a.isA<DataTypeDict>())
            return to_json_struct(*a.asD<DataTypeDict>());
        else if (a.isData())
            return nlohmann::json::parse(a.asData()->toJsonString());
        else
            return nlohmann::json();
    }

    std::string to_json(const Atom& a, int indent)
    {
        return to_json_struct(a).dump(indent);
    }

    std::string to_json(const AtomList& l, int indent)
    {
        return to_json_struct(l).dump(indent);
    }

    std::string to_json(const DataTypeString& str, int indent)
    {
        return to_json_struct(str).dump(indent);
    }

    std::string to_json(const DataTypeMList& ml, int indent)
    {
        return to_json_struct(ml.data()).dump(indent);
    }

    std::string to_json(const DataTypeDict& dict, int indent)
    {
        return to_json_struct(dict).dump(indent);
    }
}
}
