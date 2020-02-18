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
#include "datatype_json_imp.h"
#include "ceammc_log.h"

namespace ceammc {

DataTypeJsonImpl::DataTypeJsonImpl()
{
}

DataTypeJsonImpl::DataTypeJsonImpl(const DataTypeJsonImpl& imp)
    : json_(imp.json_)
{
}

DataTypeJsonImpl::DataTypeJsonImpl(const nlohmann::json& json)
    : json_(json)
{
}

DataTypeJsonImpl::DataTypeJsonImpl(nlohmann::json&& json)
    : json_(std::move(json))
{
}

std::string DataTypeJsonImpl::toString() const
{
    try {
        return json_.dump();
    } catch (nlohmann::detail::type_error& e) {
        LIB_ERR << e.what();
        return "";
    }
}

bool DataTypeJsonImpl::parse(const char* str)
{
    try {
        json_ = nlohmann::json::parse(str);
        return true;
    } catch (std::exception& e) {
        LIB_ERR << e.what();
        return false;
    }
}

bool DataTypeJsonImpl::append(const Atom& a)
{
    //    json_.
}

size_t DataTypeJsonImpl::size() const
{
    return json_.size();
}

bool DataTypeJsonImpl::empty() const
{
    return json_.empty();
}

std::string DataTypeJsonImpl::stringAt(size_t idx) const
{
    try {
        return json_.at(idx).dump();
    } catch (std::exception& e) {
        LIB_ERR << e.what();
        return {};
    }
}

bool DataTypeJsonImpl::isArray() const
{
    return json_.is_array();
}

bool DataTypeJsonImpl::isNull() const
{
    return json_.is_null();
}

void DataTypeJsonImpl::clear()
{
    json_.clear();
}

bool DataTypeJsonImpl::operator==(const DataTypeJsonImpl& impl) const
{
    return json_ == impl.json_;
}

JsonValue DataTypeJsonImpl::valueAt(size_t idx) const
{
    if (json_.empty())
        return JsonValue();

    try {
        auto j = json_.at(idx);
        if (j.is_boolean())
            return Atom(j.get<bool>() ? 1 : 0);
        else if (j.is_number())
            return Atom(j.get<t_float>());
        else if (j.is_string())
            return Atom(gensym(j.get<std::string>().c_str()));
        else if (j.is_array()) {
            // find non simple array
            auto it = std::find_if(j.begin(), j.end(),
                [](decltype(j)& el) {
                    return !el.is_primitive();
                });

            if (it != j.end()) {
                return DataTypeJson(DataTypeJsonImpl(j));
            } else {
                AtomList lst;
                lst.reserve(j.size());
                for (auto& el : j) {
                    if (el.is_boolean())
                        return Atom(el.get<bool>() ? 1 : 0);
                    else if (el.is_number())
                        return Atom(el.get<t_float>());
                    else if (el.is_string())
                        return Atom(gensym(el.get<std::string>().c_str()));
                    else {
                        LIB_ERR << "unknown non-simple type";
                        return JsonValue();
                    }
                }
            }
        } else {
            return DataTypeJson(DataTypeJsonImpl(j));
        }
    } catch (std::exception& e) {
        LIB_ERR << e.what();
        return JsonValue();
    }
}

bool DataTypeJsonImpl::addFloat(t_float f)
{
    if (json_.is_array()) {
        json_.push_back(f);
        return true;
    } else if (json_.is_null()) {
        json_ = json_.array({ f });
        return true;
    } else
        return false;
}

bool DataTypeJsonImpl::addSymbol(t_symbol* s)
{
    if (json_.is_array()) {
        json_.push_back(s->s_name);
        return true;
    } else if (json_.is_null()) {
        json_ = json_.array({ s->s_name });
        return true;
    } else
        return false;
}

bool DataTypeJsonImpl::addList(const AtomList& l)
{
    auto add_list = [this](const AtomList& l) {
        for (auto& a : l) {
            if (a.isFloat())
                json_.push_back(a.asFloat());
            else if (a.isSymbol())
                json_.push_back(a.asSymbol()->s_name);
        }
    };

    if (json_.is_array()) {
        add_list(l);
        return true;
    } else if (json_.is_null()) {
        json_ = json_.array();
        add_list(l);
        return true;
    } else
        return false;
}

}
