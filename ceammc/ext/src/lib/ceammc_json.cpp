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
#include "datatype_dict.h"
#include "datatype_mlist.h"
#include "datatype_string.h"

#include "json/json.hpp"

namespace ceammc {
namespace json {

    static nlohmann::json to_json_struct(const Atom& a, const JsonWriteOpts& opt);
    static nlohmann::json to_json_struct(const AtomList& l, const JsonWriteOpts& opt);
    static nlohmann::json to_json_struct(const DataTypeDict& dict, const JsonWriteOpts& opt);
    static nlohmann::json to_json_struct(const DataTypeString& str, const JsonWriteOpts& opt);

    static nlohmann::json to_json_struct(const AtomList& l, const JsonWriteOpts& opt)
    {
        nlohmann::json arr = nlohmann::json::array();

        for (const Atom& a : l)
            arr.push_back(to_json_struct(a, opt));

        return arr;
    }

    static nlohmann::json to_json_struct(const DataTypeDict& dict, const JsonWriteOpts& opt)
    {
        auto obj = nlohmann::json::object();

        for (auto& kv : dict) {
            auto key = kv.first->s_name;

            auto& value = kv.second;
            if (opt.compressSingleList && value.isA<Atom>())
                obj[key] = to_json_struct(value.asT<Atom>(), opt);
            else
                obj[key] = to_json_struct(value, opt);
        }

        return obj;
    }

    static nlohmann::json to_json_struct(const DataTypeString& str, const JsonWriteOpts& opt)
    {
        return nlohmann::json(str.str());
    }

    static nlohmann::json to_json_struct(const Atom& a, const JsonWriteOpts& opt)
    {
        if (a.isSymbol())
            return nlohmann::json(a.asSymbol()->s_name);
        else if (a.isInteger())
            return nlohmann::json(a.asInt());
        else if (a.isFloat())
            return nlohmann::json(a.asFloat());
        else if (a.isA<DataTypeMList>())
            return to_json_struct(a.asD<DataTypeMList>()->data(), opt);
        else if (a.isA<DataTypeString>())
            return to_json_struct(*a.asD<DataTypeString>(), opt);
        else if (a.isA<DataTypeDict>())
            return to_json_struct(*a.asD<DataTypeDict>(), opt);
        else if (a.isData())
            return nlohmann::json::parse(a.asData()->toJsonString());
        else
            return nlohmann::json();
    }

    std::string to_json_string(const Atom& a, const JsonWriteOpts& opt)
    {
        return to_json_struct(a, opt).dump(opt.indent);
    }

    std::string to_json_string(const AtomList& l, const JsonWriteOpts& opt)
    {
        return to_json_struct(l, opt).dump(opt.indent);
    }

    std::string to_json_string(const DataTypeString& str, const JsonWriteOpts& opt)
    {
        return to_json_struct(str, opt).dump(opt.indent);
    }

    std::string to_json_string(const DataTypeMList& ml, const JsonWriteOpts& opt)
    {
        return to_json_struct(ml.data(), opt).dump(opt.indent);
    }

    std::string to_json_string(const DataTypeDict& dict, const JsonWriteOpts& opt)
    {
        return to_json_struct(dict, opt).dump(opt.indent);
    }
}
}
