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

#if 1
#define DBG(msg) LIB_DBG << "|||" << msg
#else
#define DBG(msg)
#endif

DataTypeJsonImpl::DataTypeJsonImpl()
{
    DBG("empty - DataTypeJsonImpl()");
}

DataTypeJsonImpl::~DataTypeJsonImpl()
{
    DBG("~DataTypeJsonImpl()");
}

DataTypeJsonImpl::DataTypeJsonImpl(const DataTypeJsonImpl& imp)
    : json_(imp.json_)
{
    DBG("copy - DataTypeJsonImpl(const DataTypeJsonImpl&)");
}

DataTypeJsonImpl::DataTypeJsonImpl(const nlohmann::json& json)
    : json_(json)
{
    DBG("copy - DataTypeJsonImpl(const nlohmann::json& json)");
}

DataTypeJsonImpl::DataTypeJsonImpl(nlohmann::json&& json)
    : json_(std::move(json))
{
    DBG("move - DataTypeJsonImpl(nlohmann::json&& json)");
}

DataTypeJsonImpl::DataTypeJsonImpl(t_float f)
    : json_(f)
{
    DBG("float - DataTypeJsonImpl(t_float f): " << f);
}

DataTypeJsonImpl::DataTypeJsonImpl(t_symbol* s)
    : json_(s->s_name)
{
    DBG("symbol - DataTypeJsonImpl(t_symbol* s): " << s);
}

DataTypeJsonImpl::DataTypeJsonImpl(const char* str)
{
    DBG("parse - DataTypeJsonImpl(const char* str): " << str);
    parse(str);
}

DataTypeJsonImpl::DataTypeJsonImpl(const FloatList& l)
    : json_(l)
{
    DBG("float list - DataTypeJsonImpl(const FloatList &l)");
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
        std::string esc_str(str);
        for (size_t i = 0; i < esc_str.length(); i++) {
            switch (esc_str[i]) {
            case '(':
                esc_str[i] = '{';
                break;
            case ')':
                esc_str[i] = '}';
                break;
            default:
                break;
            }
        }

        json_ = nlohmann::json::parse(esc_str);
        return true;
    } catch (std::exception& e) {
        LIB_ERR << e.what();
        return false;
    }
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
    auto add_list = [](const AtomList& l, nlohmann::json& j) {
        for (auto& a : l) {
            if (a.isInteger())
                j.push_back(static_cast<long>(a.asFloat()));
            else if (a.isFloat())
                j.push_back(a.asFloat());
            else if (a.isSymbol())
                j.push_back(a.asSymbol()->s_name);
        }
    };

    if (json_.is_array()) {
        auto j = json_.array();
        add_list(l, j);
        json_.push_back(j);
        return true;
    } else if (json_.is_null()) {
        json_ = json_.array();
        add_list(l, json_);
        return true;
    } else
        return false;
}

bool DataTypeJsonImpl::addJson(const DataTypeJsonImpl& impl)
{
    if (json_.is_array()) {
        json_.push_back(impl.json_);
        return true;
    } else if (json_.is_null()) {
        json_ = impl.json_;
        return true;
    } else
        return false;
}

void DataTypeJsonImpl::setFloat(t_float f)
{
    json_ = f;
}

void DataTypeJsonImpl::setSymbol(t_symbol* s)
{
    json_ = s->s_name;
}

void DataTypeJsonImpl::set(const DataTypeJsonImpl& imp)
{
    json_ = imp.json_;
}

DataTypeJsonImpl DataTypeJsonImpl::match(const char* pattern) const
{
    try {
        auto ptr = nlohmann::json::json_pointer(pattern);
        return DataTypeJsonImpl(json_.at(ptr));
    } catch (std::exception& e) {
        LIB_ERR << e.what();
        return {};
    }
}

DataTypeJsonImpl DataTypeJsonImpl::at(size_t idx) const
{
    try {
        return json_.at(idx);
    } catch (std::exception& e) {
        LIB_ERR << e.what();
        return {};
    }
}

DataTypeJsonImpl DataTypeJsonImpl::at(const char* key) const
{
    try {
        return json_.at(key);
    } catch (std::exception& e) {
        LIB_ERR << e.what();
        return {};
    }
}

bool DataTypeJsonImpl::insertFloat(const char* key, t_float f)
{
    if (json_.is_object()) {
        json_[key] = f;
        return true;
    } else if (json_.is_null()) {
        json_ = json_.object();
        json_[key] = f;
        return true;
    } else
        return false;
}

bool DataTypeJsonImpl::insertSymbol(const char* key, t_symbol* s)
{
    if (json_.is_object()) {
        json_[key] = s->s_name;
        return true;
    } else if (json_.is_null()) {
        json_[key] = s->s_name;
        return true;
    } else
        return false;
}

bool DataTypeJsonImpl::insertJson(const char* key, const DataTypeJsonImpl& json)
{
    if (json_.is_object()) {
        json_[key] = json.json_;
        return true;
    } else if (json_.is_null()) {
        json_[key] = json.json_;
        return true;
    } else
        return false;
}

}
