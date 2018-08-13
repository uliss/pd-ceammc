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
#include "ceammc_datatypes.h"
#include "ceammc_format.h"
#include "ceammc_log.h"
#include "datatype_mlist.h"
#include "dict_parser_impl.h"

#include "json/json.hpp"

#include <fstream>

extern "C" int dict_parse_string(t_dict* dict, const char* s);

using namespace ceammc;

const DataType DataTypeDict::dataType = data::DATA_DICT;

DataTypeDict::DataTypeDict()
{
}

DataTypeDict::DataTypeDict(const DataTypeDict& dict)
    : dict_(dict.dict_)
{
}

DataTypeDict::DataTypeDict(const std::string& str)
{
    fromString(str);
}

DataTypeDict* DataTypeDict::clone() const
{
    return new DataTypeDict(*this);
}

DataType DataTypeDict::type() const
{
    return dataType;
}

std::string DataTypeDict::toString() const
{
    std::string res;

    for (auto& e : dict_) {
        res += '[';
        res += to_string_quoted(e.first);
        res += ':';

        if (e.second.type() == typeid(Atom)) {
            auto& atom = boost::get<Atom>(e.second);
            res += ' ';
            res += to_string_quoted(atom);
        } else if (e.second.type() == typeid(DataAtom)) {
            auto& data = boost::get<DataAtom>(e.second);
            if (data.isData()) {
                res += ' ';
                res += to_string_quoted(data.data()->toString());
            }
        } else if (e.second.type() == typeid(AtomList)) {
            auto& lst = boost::get<AtomList>(e.second);
            for (auto& atom : lst) {
                res += ' ';
                res += to_string_quoted(atom);
            }
        }

        res += ']';
        res += ' ';
    }

    // remove last space
    if (res.size() > 0)
        res.resize(res.size() - 1);

    return res;
}

bool DataTypeDict::isEqual(const AbstractData* d) const
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

bool DataTypeDict::operator==(const DataTypeDict& d) const
{
    return dict_ == d.dict_;
}

size_t DataTypeDict::size() const
{
    return dict_.size();
}

bool DataTypeDict::contains(const Atom& key) const
{
    return dict_.find(key) != dict_.end();
}

DictValue DataTypeDict::value(const Atom& key) const
{
    auto it = dict_.find(key);

    return it == dict_.end() ? DictValue() : DictValue(it->second);
}

void DataTypeDict::insert(const std::string& key, const std::string& value)
{
    dict_[Atom(gensym(key.c_str()))] = Atom(gensym(value.c_str()));
}

void DataTypeDict::insert(const std::string& key, t_float value)
{
    insert(Atom(gensym(key.c_str())), value);
}

void DataTypeDict::insert(const std::string& key, const Atom& value)
{
    insert(Atom(gensym(key.c_str())), value);
}

void DataTypeDict::insert(const Atom& key, t_float value)
{
    insert(key, Atom(value));
}

void DataTypeDict::insert(const Atom& key, const Atom& value)
{
    dict_[key] = value;
}

void DataTypeDict::insert(const Atom& key, const AtomList& value)
{
    dict_[key] = value;
}

void DataTypeDict::insert(const Atom& key, const DataAtom& value)
{
    dict_[key] = value;
}

bool DataTypeDict::remove(const Atom& key)
{
    auto it = dict_.find(key);

    if (it == dict_.end())
        return false;

    dict_.erase(it);
    return true;
}

void DataTypeDict::clear()
{
    dict_.clear();
}

bool DataTypeDict::fromString(const std::string& str)
{
    t_dict* d = dict_new();

    if (dict_parse_string(d, str.c_str()) != 0) {
        dict_free(d);
        return false;
    }

    *this = dict_get(d);
    dict_free(d);
    return true;
}

MaybeString DataTypeDict::toJSON(int indent) const
{
    using json = nlohmann::json;
    json j;

    try {
        for (auto& kv : dict_) {
            auto key = to_string(kv.first);
            auto& value = kv.second;

            if (value.type() == typeid(Atom)) {
                auto& atom = boost::get<Atom>(value);
                if (atom.isFloat())
                    j[key] = atom.asFloat();
                else
                    j[key] = to_string(atom);
            } else if (value.type() == typeid(AtomList)) {
                auto& lst = boost::get<AtomList>(value);

                auto array = json::array();
                for (auto& atom : lst) {
                    if (atom.isFloat())
                        array.push_back(atom.asFloat());
                    else
                        array.push_back(to_string(atom));
                }

                j[key] = array;
            } else if (value.type() == typeid(DataAtom)) {
                LIB_DBG << "[dict] Data types are not supported at this moment. Storing as string";
                auto& data = boost::get<DataAtom>(value);

                if (data.data()->type() == DataTypeDict::dataType) {
                    auto d = data.data()->as<DataTypeDict>();
                    auto str = d->toJSON(indent);
                    if (str) {
                        j[key] = json::parse(*str);
                    } else {
                        LIB_ERR << "can't convert to JSON: " << d->toString();
                    }
                } else
                    j[key] = to_string(data.data());
            }
        }

        if (j.empty())
            return boost::none;

        return j.dump(indent);
    } catch (json::exception& e) {
        LIB_ERR << "[dict] JSON exception: " << e.what();
        return boost::none;
    }
}

static void from_json(const nlohmann::json& j, DictValue& p)
{
    if (j.is_boolean())
        p = Atom(j.get<bool>() ? 1 : 0);
    else if (j.is_number())
        p = Atom(j.get<t_float>());
    else if (j.is_string())
        p = Atom(gensym(j.get<std::string>().c_str()));
    else if (j.is_array()) {

        bool simple_array = true;
        for (auto& el : j) {
            if (!el.is_primitive()) {
                simple_array = false;
                break;
            }
        }

        if (simple_array) {
            AtomList lst;
            for (auto& el : j) {
                DictValue v;
                from_json(el, v);
                lst.append(boost::get<Atom>(v));
            }

            p = lst;
        } else {
            DataTypeMList* ptr = new DataTypeMList;
            DataTPtr<DataTypeMList> dptr(ptr);

            for (auto& el : j) {
                DictValue v;
                from_json(el, v);

                if (v.type() == typeid(Atom))
                    ptr->append(boost::get<Atom>(v));
                else if (v.type() == typeid(DataAtom))
                    ptr->append(boost::get<DataAtom>(v));
                else if (v.type() == typeid(AtomList)) {
                    // simple nested array
                    DataTypeMList* ptr2 = new DataTypeMList;
                    DataTPtr<DataTypeMList> dptr2(ptr2);
                    auto& inner_list = boost::get<AtomList>(v);
                    for (auto& el : inner_list)
                        ptr2->append(el);

                    ptr->append(DataAtom(ptr2));
                } else {
                    LIB_ERR << "[dict] JSON: unknown type: " << v.type().name();
                }
            }

            p = DataAtom(dptr);
        }
    } else if (j.is_object()) {
        DataTypeDict* ptr = new DataTypeDict;
        DataTPtr<DataTypeDict> dptr(ptr);

        for (auto it = j.begin(); it != j.end(); ++it) {
            DictValue v;
            from_json(*it, v);
            auto key = Atom(gensym(it.key().c_str()));
            ptr->innerData()[key] = v;
        }

        p = DataAtom(dptr);
    } else {
        p = Atom();
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

        for (auto it = j.begin(); it != j.end(); ++it) {
            DictValue v;
            from_json(*it, v);
            std::string str_key = it.key();
            Atom atom_key;

            auto bb = binbuf_new();
            binbuf_text(bb, const_cast<char*>(str_key.c_str()), str_key.size());

            // if single key
            if (binbuf_getnatom(bb) == 1) {
                auto bb_atom = binbuf_getvec(bb);
                if (bb_atom)
                    atom_key = Atom(*bb_atom);
            }

            binbuf_free(bb);

            if (atom_key.isNone())
                atom_key = Atom(gensym(str_key.c_str()));

            dict_[atom_key] = v;
        }
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

bool DataTypeDict::isNull(const DictValue& v)
{
    return v.which() == 0;
}

bool DataTypeDict::isAtom(const DictValue& v)
{
    return isType<Atom>(v);
}

bool DataTypeDict::isAtomList(const DictValue& v)
{
    return isType<AtomList>(v);
}

bool DataTypeDict::isDataAtom(const DictValue& v)
{
    return isType<DataAtom>(v);
}

bool ceammc::to_outlet(t_outlet* x, const DictValue& v)
{
    if (DataTypeDict::isNull(v))
        return false;
    else if (v.type() == typeid(Atom))
        return to_outlet(x, boost::get<Atom>(v));
    else if (v.type() == typeid(AtomList))
        return to_outlet(x, boost::get<AtomList>(v));
    else if (v.type() == typeid(DataAtom))
        return to_outlet(x, boost::get<DataAtom>(v));
    else
        return false;
}
