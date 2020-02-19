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
#include "datatype_tree_imp.h"
#include "ceammc_log.h"

namespace ceammc {

#if 1
#define DBG(msg) LIB_DBG << "|||" << msg
#else
#define DBG(msg)
#endif

DataTypeTreeImpl::DataTypeTreeImpl()
{
    DBG("empty - DataTypeJsonImpl()");
}

DataTypeTreeImpl::~DataTypeTreeImpl()
{
    DBG("~DataTypeJsonImpl()");
}

DataTypeTreeImpl::DataTypeTreeImpl(const DataTypeTreeImpl& imp)
    : json_(imp.json_)
{
    DBG("copy - DataTypeJsonImpl(const DataTypeJsonImpl&)");
}

DataTypeTreeImpl::DataTypeTreeImpl(const nlohmann::json& json)
    : json_(json)
{
    DBG("copy - DataTypeJsonImpl(const nlohmann::json& json)");
}

DataTypeTreeImpl::DataTypeTreeImpl(nlohmann::json&& json)
    : json_(std::move(json))
{
    DBG("move - DataTypeJsonImpl(nlohmann::json&& json)");
}

DataTypeTreeImpl::DataTypeTreeImpl(t_float f)
    : json_(f)
{
    DBG("float - DataTypeJsonImpl(t_float f): " << f);
}

DataTypeTreeImpl::DataTypeTreeImpl(t_symbol* s)
    : json_(s->s_name)
{
    DBG("symbol - DataTypeJsonImpl(t_symbol* s): " << s);
}

DataTypeTreeImpl::DataTypeTreeImpl(const char* str)
{
    DBG("parse - DataTypeJsonImpl(const char* str): " << str);
    parse(str);
}

DataTypeTreeImpl::DataTypeTreeImpl(const FloatList& l)
    : json_(l)
{
    DBG("float list - DataTypeJsonImpl(const FloatList &l)");
}

std::string DataTypeTreeImpl::toString() const
{
    try {
        return json_.dump();
    } catch (nlohmann::detail::type_error& e) {
        LIB_ERR << e.what();
        return "";
    }
}

bool DataTypeTreeImpl::parse(const char* str)
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

size_t DataTypeTreeImpl::size() const
{
    return json_.size();
}

bool DataTypeTreeImpl::empty() const
{
    return json_.empty();
}

bool DataTypeTreeImpl::isArray() const
{
    return json_.is_array();
}

bool DataTypeTreeImpl::isNull() const
{
    return json_.is_null();
}

void DataTypeTreeImpl::clear()
{
    json_.clear();
}

bool DataTypeTreeImpl::operator==(const DataTypeTreeImpl& impl) const
{
    return json_ == impl.json_;
}

bool DataTypeTreeImpl::addFloat(t_float f)
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

bool DataTypeTreeImpl::addSymbol(t_symbol* s)
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

bool DataTypeTreeImpl::addList(const AtomList& l)
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

bool DataTypeTreeImpl::addJson(const DataTypeTreeImpl& impl)
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

void DataTypeTreeImpl::setFloat(t_float f)
{
    json_ = f;
}

void DataTypeTreeImpl::setSymbol(t_symbol* s)
{
    json_ = s->s_name;
}

void DataTypeTreeImpl::set(const DataTypeTreeImpl& imp)
{
    json_ = imp.json_;
}

DataTypeTreeImpl DataTypeTreeImpl::match(const char* pattern) const
{
    try {
        auto ptr = nlohmann::json::json_pointer(pattern);
        return DataTypeTreeImpl(json_.at(ptr));
    } catch (std::exception& e) {
        LIB_ERR << e.what();
        return {};
    }
}

DataTypeTreeImpl DataTypeTreeImpl::at(size_t idx) const
{
    try {
        return json_.at(idx);
    } catch (std::exception& e) {
        LIB_ERR << e.what();
        return {};
    }
}

DataTypeTreeImpl DataTypeTreeImpl::at(const char* key) const
{
    try {
        return json_.at(key);
    } catch (std::exception& e) {
        LIB_ERR << e.what();
        return {};
    }
}

bool DataTypeTreeImpl::insertFloat(const char* key, t_float f)
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

bool DataTypeTreeImpl::insertSymbol(const char* key, t_symbol* s)
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

bool DataTypeTreeImpl::insertJson(const char* key, const DataTypeTreeImpl& json)
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
