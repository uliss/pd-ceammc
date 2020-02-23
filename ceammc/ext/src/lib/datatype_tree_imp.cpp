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
#include "ceammc_dataatom.h"
#include "ceammc_log.h"
#include "datatype_tree.h"
#include "fmt/format.h"

namespace ceammc {

#if 0
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
    DBG("~DataTypeJsonImpl() " << toString());
}

DataTypeTreeImpl::DataTypeTreeImpl(const DataTypeTreeImpl& imp)
    : json_(imp.json_)
{
    DBG("copy - DataTypeJsonImpl(const DataTypeJsonImpl&) " << imp.toString());
}

DataTypeTreeImpl::DataTypeTreeImpl(DataTypeTreeImpl&& imp)
    : json_(std::move(imp.json_))
{
    DBG("move - DataTypeJsonImpl(DataTypeTreeImpl&&) " << json_.dump());
}

DataTypeTreeImpl::DataTypeTreeImpl(const nlohmann::json& json)
    : json_(json)
{
    DBG("copy - DataTypeJsonImpl(const nlohmann::json& json) " << json_.dump());
}

DataTypeTreeImpl::DataTypeTreeImpl(nlohmann::json&& json)
    : json_(std::move(json))
{
    DBG("move - DataTypeJsonImpl(nlohmann::json&& json) " << json_.dump());
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

DataTypeTreeImpl::DataTypeTreeImpl(const char* s)
    : json_(s)
{
    DBG("string - DataTypeJsonImpl(const char* s): " << s);
}

DataTypeTreeImpl::DataTypeTreeImpl(const FloatList& l)
    : json_(l)
{
    DBG("float list - DataTypeJsonImpl(const FloatList &l)");
}

DataTypeTreeImpl::DataTypeTreeImpl(const AtomList& l)
    : json_(nlohmann::json::array())
{
    for (auto& a : l) {
        if (a.isFloat())
            json_.push_back(a.asFloat());
        else if (a.isSymbol())
            json_.push_back(a.asSymbol()->s_name);
    }

    DBG("atom list - DataTypeJsonImpl(const AtomList &l)");
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

DataTypeTreeImpl DataTypeTreeImpl::fromString(const char* str)
{
    try {
        return { nlohmann::json::parse(treeToJson(str)) };
    } catch (std::exception& e) {
        LIB_ERR << e.what();
        LIB_ERR << "\tjson: " << treeToJson(str);
        return {};
    }
}

std::string DataTypeTreeImpl::treeToJson(const char* tree_str)
{
    std::string res(tree_str);

    bool in_string = false;
    bool escape = false;

    for (size_t i = 0; i < res.length(); i++) {
        switch (res[i]) {
        case '"':
            if (in_string && !escape)
                in_string = false;
            else
                in_string = true;

            break;
        case '(':
            if (!in_string)
                res[i] = '{';
            break;
        case ')':
            if (!in_string)
                res[i] = '}';
            break;
        case '\\':
            escape = !escape;
            continue;
        }

        escape = false;
    }

    return res;
}

std::string DataTypeTreeImpl::jsonToTree(const std::string& json_str)
{
    std::string res;
    res.reserve(json_str.length());

    bool in_string = false;

    for (size_t i = 0; i < json_str.length(); i++) {
        switch (json_str[i]) {
        case '"':
            in_string = !in_string;
            break;
        case '{':
            if (!in_string) {
                res.push_back('(');
                continue;
            }
            break;
        case '}':
            if (!in_string) {
                res.push_back(')');
                continue;
            }
            break;
        default:
            break;
        }

        res.push_back(json_str[i]);
    }

    return res;
}

AtomList DataTypeTreeImpl::toList(const nlohmann::json& json)
{
    AtomList res;
    res.reserve(json.size());

    for (auto& el : json)
        res.append(toAtom(el));

    return res;
}

Atom DataTypeTreeImpl::toAtom(const nlohmann::json& json)
{
    if (json.is_number())
        return Atom(json.get<t_float>());
    else if (json.is_string())
        return Atom(gensym(json.get<std::string>().c_str()));
    else if (json.is_boolean())
        return Atom(json.get<bool>() ? 1 : 0.f);
    else if (json.is_array())
        return DataPtr(new DataTypeTree(DataTypeTreeImpl(json))).asAtom();
    else if (json.is_object())
        return DataPtr(new DataTypeTree(DataTypeTreeImpl(json))).asAtom();
    else
        return Atom();
}

bool DataTypeTreeImpl::parse(const char* str)
{
    try {
        json_ = nlohmann::json::parse(treeToJson(str));
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

bool DataTypeTreeImpl::isSimpleArray() const
{
    for (auto& e : json_) {
        if (!e.is_primitive())
            return false;
    }

    return true;
}

bool DataTypeTreeImpl::isNull() const
{
    return json_.is_null();
}

bool DataTypeTreeImpl::isFloat() const
{
    return json_.is_number() || json_.is_boolean();
}

bool DataTypeTreeImpl::isString() const
{
    return json_.is_string();
}

bool DataTypeTreeImpl::isObject() const
{
    return json_.is_object();
}

const char* DataTypeTreeImpl::typeName() const
{
    return json_.type_name();
}

t_float DataTypeTreeImpl::asFloat() const
{
    return json_.get<t_float>();
}

t_symbol* DataTypeTreeImpl::asSymbol() const
{
    return gensym(json_.get<std::string>().c_str());
}

AtomList DataTypeTreeImpl::asAtomList() const
{
    AtomList res;
    if (!json_.is_array())
        return res;

    res.reserve(json_.size());

    for (auto& j : json_) {
        if (j.is_number())
            res.append(Atom(j.get<t_float>()));
        else if (j.is_boolean())
            res.append(Atom(j.get<bool>() ? 1 : 0));
        else if (j.is_string())
            res.append(Atom(gensym(j.get<std::string>().c_str())));
    }

    return res;
}

EitherImpTreeFloat DataTypeTreeImpl::getFloat() const
{
    if (json_.is_number() || json_.is_boolean())
        return json_.get<t_float>();
    else
        return EitherImpTreeFloat::makeError(fmt::format("float expected instead of {0}", json_.type_name()));
}

EitherImpTreeString DataTypeTreeImpl::getString() const
{
    if (json_.is_string())
        return json_.get<std::string>();
    else
        return EitherImpTreeString::makeError(fmt::format("string expected instead of {0}", json_.type_name()));
}

EitherImpTree DataTypeTreeImpl::getArray() const
{
    if (json_.is_array())
        return json_;
    else
        return EitherImpTree::makeError(fmt::format("list expected instead of {0}", json_.type_name()));
}

EitherImpTree DataTypeTreeImpl::getObject() const
{
    if (json_.is_object())
        return json_;
    else
        return EitherImpTree::makeError(fmt::format("dict expected instead of {0}", json_.type_name()));
}

TreeImpKeyList DataTypeTreeImpl::keys() const
{
    TreeImpKeyList keys;

    if (json_.is_object()) {
        keys.reserve(json_.size());

        for (auto it = json_.begin(); it != json_.end(); ++it)
            keys.push_back(it.key());
    }

    return keys;
}

void DataTypeTreeImpl::clear()
{
    json_.clear();
}

bool DataTypeTreeImpl::operator==(const DataTypeTreeImpl& impl) const
{
    return json_ == impl.json_;
}

bool DataTypeTreeImpl::arrayAdd(t_float f)
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

bool DataTypeTreeImpl::arrayAdd(t_symbol* s)
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

bool DataTypeTreeImpl::arrayAdd(const AtomList& l)
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

bool DataTypeTreeImpl::arrayAdd(const DataTypeTreeImpl& impl)
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

void DataTypeTreeImpl::setArray()
{
    json_ = nlohmann::json::array();
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
        LIB_ERR << "can't get element at " << idx << ":\n\t" << e.what();
        return {};
    }
}

DataTypeTreeImpl DataTypeTreeImpl::at(const char* key) const
{
    try {
        return json_.at(key);
    } catch (std::exception& e) {
        LIB_ERR << "can't get key '" << key << "':\n\t" << e.what();
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

bool DataTypeTreeImpl::insertTree(const char* key, const DataTypeTreeImpl& tree)
{
    if (json_.is_object()) {
        json_[key] = tree.json_;
        return true;
    } else if (json_.is_null()) {
        json_[key] = tree.json_;
        return true;
    } else
        return false;
}

}
