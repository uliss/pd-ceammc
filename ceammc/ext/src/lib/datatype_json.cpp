#include "datatype_json.h"
#include "ceammc_data.h"
#include "ceammc_log.h"
#include "datatype_mlist.h"
#include "fmt/core.h"

namespace ceammc {

void to_json(nlohmann::json& out, const Atom& atom)
{
    using namespace ceammc;

    try {

        if (atom.isFloat() && !atom.isInteger())
            out = atom.asFloat();
        else if (atom.isInteger())
            out = atom.asInt();
        else if (atom.isSymbol())
            out = atom.asSymbol()->s_name;
        else if (atom.isA<DataTypeDict>())
            out = nlohmann::json(*atom.asD<DataTypeDict>());
        else if (atom.isData())
            out = nlohmann::json::parse(atom.asData()->toJsonString());
        else
            out = nlohmann::json();

    } catch (nlohmann::json::exception& e) {
        LIB_ERR << "[dict] JSON exception: " << e.what();
        out = nlohmann::json();
    }
}

void to_json(nlohmann::json& out, const AtomList& lst)
{
    out = nlohmann::json::array();

    for (auto& x : lst)
        out.push_back(nlohmann::json(x));
}

void to_json(nlohmann::json& out, const DataTypeDict& dict)
{
    if (dict.size() == 0) {
        out = nlohmann::json::object();
    } else {
        for (auto& kv : dict) {
            auto key = kv.first->s_name;
            auto& value = kv.second;

            if (value.isAtom())
                out[key] = nlohmann::json(value[0]);
            else
                out[key] = nlohmann::json(value);
        }
    }
}

void from_json(const nlohmann::json& j, AtomList& lst)
{
    bool simple_array = std::all_of(j.begin(), j.end(),
        [](const decltype(j.begin())::value_type& v) { return v.is_primitive(); });

    if (simple_array) {
        for (auto& x : j)
            lst.append(x.get<Atom>());
    } else {
        DataTypeMList* ptr = new DataTypeMList;
        Atom mlist(ptr);

        for (auto& x : j)
            ptr->append(x.get<Atom>());

        lst.append(mlist);
    }
}

void from_json(const nlohmann::json& json, DataTypeMList& ml)
{
    for (auto& x : json)
        ml.append(x.get<Atom>());
}

void from_json(const nlohmann::json& j, Atom& atom)
{
    if (j.is_boolean())
        atom = Atom(j.get<bool>() ? 1 : 0);
    else if (j.is_number())
        atom = Atom(j.get<t_float>());
    else if (j.is_string())
        atom = Atom(gensym(j.get<std::string>().c_str()));
    else if (j.is_array()) {
        auto ml = new DataTypeMList;
        j.get_to<DataTypeMList>(*ml);
        atom = Atom(ml);
    } else if (j.is_object()) {
        auto d = new DataTypeDict;
        j.get_to<DataTypeDict>(*d);
        atom = Atom(d);
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
        auto key = gensym(it.key().c_str());

        if (it->is_boolean() || it->is_number() || it->is_string() || it->is_object())
            dict[key] = AtomList(it->get<Atom>());
        else if (it->is_array())
            dict[key] = it->get<AtomList>();
        else
            dict[key] = AtomList();
    }
}

}
