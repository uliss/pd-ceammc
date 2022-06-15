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
#include "datatype_dict.h"
#include "ceammc_data.h"
#include "ceammc_datastorage.h"
#include "ceammc_datatypes.h"
#include "ceammc_format.h"
#include "ceammc_json.h"
#include "ceammc_log.h"
#include "ceammc_output.h"
#include "datatype_mlist.h"
#include "datatype_string.h"
#include "fmt/format.h"
#include "json/json.hpp"

#include <ctime>
#include <fstream>
#include <random>

namespace ceammc {

void to_json(nlohmann::json& out, const Atom& atom);
void to_json(nlohmann::json& out, const AtomList& lst);
void to_json(nlohmann::json& out, const DataTypeDict& dict);

void from_json(const nlohmann::json& j, Atom& atom);
void from_json(const nlohmann::json& json, AtomList& lst);
void from_json(const nlohmann::json& j, DataTypeDict::DictMap& dict);

static t_symbol* atom_to_symbol(const Atom& a)
{
    if (a.isSymbol())
        return a.asSymbol();
    else
        return gensym(to_string(a).c_str());
}

static Atom newFromDict(const Dict& d)
{
    auto dict = new DataTypeDict;

    for (auto& kv : d) {
        auto key = gensym(kv.first.c_str());
        if (kv.second.isAtom())
            dict->insert(key, kv.second.asT<Atom>());
        else
            dict->insert(key, kv.second);
    }

    return dict;
}

const int DataTypeDict::dataType = DataStorage::instance().registerNewType("Dict", nullptr, newFromDict);

DataTypeDict::DataTypeDict() noexcept = default;

DataTypeDict::DataTypeDict(const DataTypeDict& dict)
    : dict_(dict.dict_)
{
}

DataTypeDict::DataTypeDict(DataTypeDict&& dict) noexcept
    : dict_(std::move(dict.dict_))
{
}

DataTypeDict::DataTypeDict(const std::string& str)
{
    fromString(str);
}

DataTypeDict::DataTypeDict(std::initializer_list<DictKeyValue> pairs)
{
    for (auto& p : pairs)
        dict_.insert(p);
}

DataTypeDict& DataTypeDict::operator=(const DataTypeDict& dict)
{
    dict_ = dict.dict_;
    return *this;
}

DataTypeDict& DataTypeDict::operator=(DataTypeDict&& dict) noexcept
{
    dict_ = std::move(dict.dict_);
    return *this;
}

DataTypeDict* DataTypeDict::clone() const
{
    return new DataTypeDict(*this);
}

int DataTypeDict::type() const noexcept
{
    return dataType;
}

std::string DataTypeDict::toString() const
{
    std::string res;

    res += '[';
    for (auto& kv : dict_) {
        res += to_string_quoted(kv.first);
        res += ':';

        for (auto& atom : kv.second) {
            res += ' ';
            res += to_string_quoted(atom);
        }

        res += ' ';
    }

    // remove last space
    if (res.size() > 1)
        res.resize(res.size() - 1);

    res += ']';

    return res;
}

std::string DataTypeDict::valueToJsonString() const
{
    return json::to_json(*this);
}

bool DataTypeDict::isEqual(const AbstractData* d) const noexcept
{
    auto data = d->as<DataTypeDict>();

    // not a dict
    if (!data)
        return false;

    // self check
    if (data == this)
        return true;

    return data->dict_ == dict_;
}

bool DataTypeDict::operator==(const DataTypeDict& d) const noexcept
{
    return dict_ == d.dict_;
}

void DataTypeDict::removeIf(std::function<bool(t_symbol*)> key_pred)
{
    for (auto it = dict_.cbegin(); it != dict_.cend(); /* increment in loop body */) {
        if (key_pred(it->first))
            it = dict_.erase(it);
        else
            ++it;
    }
}

AtomList DataTypeDict::keys() const
{
    AtomList res;
    res.reserve(dict_.size());

    for (auto& kv : dict_)
        res.append(kv.first);

    return res;
}

AtomList DataTypeDict::flattenToList() const
{
    AtomList res;
    res.reserve(dict_.size() * 2);

    for (auto& kv : dict_) {
        res.append(kv.first);
        res.append(kv.second);
    }

    return res;
}

const AtomList& DataTypeDict::at(t_symbol* key) const
{
    auto it = dict_.find(key);
    if (it != dict_.end())
        return it->second;
    else
        throw std::out_of_range(fmt::format("key not found: {}", key->s_name));
}

AtomList& DataTypeDict::at(t_symbol* key)
{
    auto it = dict_.find(key);
    if (it != dict_.end())
        return it->second;
    else
        throw std::out_of_range(fmt::format("key not found: {}", key->s_name));
}

bool DataTypeDict::remove(t_symbol* key)
{
    auto it = dict_.find(key);
    if (it == dict_.end())
        return false;

    dict_.erase(it);
    return true;
}

bool DataTypeDict::fromString(const std::string& str)
{
    auto ob_pos = str.find('[');
    if (ob_pos == std::string::npos) {
        LIB_ERR << "can't parse dict string: " << str;
        return false;
    }

    auto parse_result = parseDataString(str.substr(ob_pos));
    if (!parse_result) {
        LIB_ERR << "parse error: " << parse_result.err();
        return false;
    }

    if (parse_result.result().isA<DataTypeDict>()) {
        *this = *(parse_result.result().asD<DataTypeDict>());
        return true;
    } else {
        LIB_ERR << "dict is expected, got: " << str;
        return false;
    }
}

void to_json(nlohmann::json& out, const DataTypeDict& dict)
{
    using json = nlohmann::json;
    for (auto& kv : dict) {
        auto key = kv.first->s_name;
        auto& value = kv.second;

        if (value.isAtom())
            out[key] = json(value[0]);
        else
            out[key] = json(value);
    }
}

void to_json(nlohmann::json& out, const Atom& atom)
{
    using json = nlohmann::json;

    try {

        if (atom.isFloat() && !atom.isInteger())
            out = atom.asFloat();
        else if (atom.isInteger())
            out = atom.asInt();
        else if (atom.isSymbol())
            out = atom.asSymbol()->s_name;
        else if (atom.isA<DataTypeString>())
            out = atom.asD<DataTypeString>()->str();
        else if (atom.isA<DataTypeDict>())
            out = json(*atom.asD<DataTypeDict>());
        else if (atom.isData())
            out = json::parse(atom.asData()->valueToJsonString());
        else
            out = json();

    } catch (json::exception& e) {
        LIB_ERR << "[dict] JSON exception: " << e.what();
        out = json();
    }
}

void to_json(nlohmann::json& out, const AtomList& lst)
{
    using json = nlohmann::json;
    out = json::array();

    for (auto& x : lst)
        out.push_back(json(x));
}

MaybeString DataTypeDict::toJSON(int indent) const
{
    using json = nlohmann::json;
    json j(*this);

    if (j.empty())
        return {};

    return j.dump(indent);
}

void from_json(const nlohmann::json& json, AtomList& lst)
{
    bool simple_array = std::all_of(json.begin(), json.end(),
        [](const decltype(json.begin())::value_type& v) { return v.is_primitive(); });

    if (simple_array) {
        for (auto& x : json)
            lst.append(x.get<Atom>());
    } else {
        DataTypeMList* ptr = new DataTypeMList;
        Atom mlist(ptr);

        for (auto& x : json)
            ptr->append(x.get<Atom>());
    }
}

void from_json(const nlohmann::json& j, Atom& atom)
{
    if (j.is_boolean())
        atom = Atom(j.get<bool>() ? 1 : 0);
    else if (j.is_number())
        atom = Atom(j.get<t_float>());
    else if (j.is_string())
        atom = Atom(gensym(j.get<std::string>().c_str()));
    else if (j.is_object()) {
        DataTypeDict* d = new DataTypeDict;
        atom = Atom(d);
        *d = j.get<DataTypeDict>();
    } else
        atom = Atom();
}

void from_json(const nlohmann::json& j, DataTypeDict& dict)
{
    from_json(j, dict.innerData());
}

void from_json(const nlohmann::json& j, DataTypeDict::DictMap& dict)
{
    for (auto it = j.begin(); it != j.end(); ++it) {
        t_symbol* key = gensym(it.key().c_str());

        if (it->is_boolean() || it->is_number() || it->is_string() || it->is_object())
            dict[key] = AtomList(it->get<Atom>());
        else if (it->is_array())
            dict[key] = it->get<AtomList>();
        else
            dict[key] = AtomList();
    }
}

bool DataTypeDict::fromJSON(const std::string& str)
{
    using json = nlohmann::json;
    try {

        json j = json::parse(str);

        if (j.empty())
            return false;

        dict_.clear();
        dict_ = j.get<decltype(dict_)>();

    } catch (json::exception& e) {
        std::cerr << "[dict] JSON exception: " << e.what() << ", while parsing: " << str;
        return false;
    }

    return true;
}

bool DataTypeDict::read(const std::string& path)
{
    std::ifstream fs(path);
    if (!fs) {
        LIB_ERR << "can not open file: " << path;
        return false;
    }

    std::string str;

    // reserve size for string
    fs.seekg(0, std::ios::end);
    str.reserve(fs.tellg());
    fs.seekg(0, std::ios::beg);

    // read to string
    str.assign((std::istreambuf_iterator<char>(fs)), std::istreambuf_iterator<char>());

    if (!fromJSON(str)) {
        LIB_ERR << "can not parse JSON file: " << path;
        return false;
    }

    return true;
}

bool DataTypeDict::write(const std::string& path) const
{
    std::ofstream ofs(path.c_str());
    // can't open
    if (!ofs) {
        LIB_ERR << "can not open file: " << path;
        return false;
    }

    auto res = toJSON(4);
    if (!res)
        return false;

    ofs << *res;
    return true;
}

bool DataTypeDict::choose(Atom& key) const noexcept
{
    if (dict_.empty())
        return false;

    const auto N = dict_.size();
    std::mt19937 gen(time(0));
    const auto offset = std::uniform_int_distribution<size_t>(0, N - 1)(gen);
    auto it = dict_.begin();

    for (size_t i = 0; i < offset; i++)
        ++it;

    key = it->first;
    return true;
}

DataTypeDict DataTypeDict::fromList(const AtomList& l, size_t step)
{
    DataTypeDict res;

    if (step == 0) {
        LIB_ERR << "invalid step: " << step;
        return res;
    }

    const size_t N = l.size();
    const size_t T = (N / step) * step;

    if (step == 1) {
        for (size_t i = 0; i < T; i++)
            res.insert(atom_to_symbol(l[i]), AtomList());
    } else if (step == 2) {
        for (size_t i = 0; i < T; i += 2)
            res.insert(atom_to_symbol(l[i]), l[i + 1]);
    } else {
        for (size_t i = 0; i < T; i += step)
            res.insert(atom_to_symbol(l[i]), l.slice(i + 1, i + step - 1));
    }

    return res;
}

std::ostream& operator<<(std::ostream& os, const DataTypeDict& dict)
{
    os << dict.toString();
    return os;
}
}
