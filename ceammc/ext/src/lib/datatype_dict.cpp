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
#include "ceammc_filesystem.h"
#include "ceammc_format.h"
#include "ceammc_json.h"
#include "ceammc_log.h"
#include "ceammc_string.h"
#include "datatype_json.h"
#include "fmt/core.h"

#include <ctime>
#include <fstream>
#include <random>

constexpr const char* TYPE_NAME = "Dict";

namespace ceammc {

static t_symbol* atom_to_symbol(const Atom& a)
{
    if (a.isSymbol())
        return a.asSymbol();
    else
        return gensym(to_string(a).c_str());
}

DataTypeId DataTypeDict::staticType()
{
    static DataTypeId id = DataStorage::instance().registerNewType(TYPE_NAME,
        nullptr,
        [](const DictAtom& datom) -> Atom { return datom; });

    return id;
}

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
    setFromDataString(str);
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

DataTypeId DataTypeDict::type() const noexcept
{
    return staticType();
}

std::string DataTypeDict::toString() const
{
    return fmt::format("[{}]", toDictStringContent());
}

std::string DataTypeDict::toJsonString() const
{
    return json::to_json_string(*this);
}

std::string DataTypeDict::toListStringContent() const
{
    string::MediumString res;
    string::SmallString buf;

    for (auto& kv : dict_) {
        auto k = kv.first->s_name;
        buf.insert(buf.end(), k, k + strlen(k));
        buf.push_back(' ');
        buf.push_back('(');

        for (auto& atom : kv.second) {
            string::parsed_atom_to_string(atom, buf);
            buf.push_back(' ');
        }

        // replace traling space to closing bracket
        if (buf.size() > 0 && buf.back() == ' ')
            buf.back() = ')';
        else
            buf.push_back(')');

        buf.push_back(' ');
        res.insert(res.end(), buf.data(), buf.data() + buf.size());
        buf.clear();
    }

    // remove last space
    if (res.size() > 0)
        res.pop_back();

    return std::string(res.data(), res.size());
}

std::string DataTypeDict::toDictStringContent() const
{
    string::MediumString res;
    string::SmallString buf;

    for (auto& kv : dict_) {
        auto k = kv.first->s_name;
        buf.insert(buf.end(), k, k + strlen(k));
        buf.push_back(':');

        for (auto& atom : kv.second) {
            buf.push_back(' ');
            string::parsed_atom_to_string(atom, buf);
        }

        buf.push_back(' ');
        res.insert(res.end(), buf.data(), buf.data() + buf.size());
        buf.clear();
    }

    // remove last space
    if (res.size() > 0)
        res.pop_back();

    return std::string(res.data(), res.size());
}

bool DataTypeDict::set(const AbstractData* d) noexcept
{
    return setDataT<DataTypeDict>(d);
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

MaybeString DataTypeDict::toJSON(int indent, bool compressSingleList) const
{
    if (dict_.empty())
        return {};

    json::JsonWriteOpts opts;
    opts.indent = indent;
    opts.compressSingleList = compressSingleList;

    return json::to_json_string(*this, opts);
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
    auto res = fs::readFileContent(path.c_str());
    RuntimeError err;
    if (res.matchError(err)) {
        LIB_ERR << err.what();
        return false;
    }

    if (!fromJSON(res.value())) {
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

DataTypeDict DataTypeDict::fromList(const AtomListView& lv, size_t step)
{
    DataTypeDict res;

    if (step == 0) {
        LIB_ERR << "invalid step: " << step;
        return res;
    }

    const size_t N = lv.size();
    const size_t T = (N / step) * step;

    if (step == 1) {
        for (size_t i = 0; i < T; i++)
            res.insert(atom_to_symbol(lv[i]), AtomList());
    } else if (step == 2) {
        for (size_t i = 0; i < T; i += 2)
            res.insert(atom_to_symbol(lv[i]), lv[i + 1]);
    } else {
        for (size_t i = 0; i < T; i += step)
            res.insert(atom_to_symbol(lv[i]), lv.subView(i + 1, i + step - 1));
    }

    return res;
}

std::ostream& operator<<(std::ostream& os, const DataTypeDict& dict)
{
    os << dict.toString();
    return os;
}
}
