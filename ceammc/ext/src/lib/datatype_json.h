#ifndef DATATYPE_JSON_H
#define DATATYPE_JSON_H

#include "datatype_dict.h"
#include "json/json.hpp"

namespace ceammc {

class DataTypeMList;

void to_json(nlohmann::json& out, const Atom& atom);
void to_json(nlohmann::json& out, const AtomList& lst);
void to_json(nlohmann::json& out, const DataTypeDict& dict);
void to_json(nlohmann::json& out, const DataTypeMList& ml);

void from_json(const nlohmann::json& j, Atom& atom);
void from_json(const nlohmann::json& j, AtomList& lst);
void from_json(const nlohmann::json& j, DataTypeDict& dict);
void from_json(const nlohmann::json& j, DataTypeMList& dict);
void from_json(const nlohmann::json& j, DataTypeDict::DictMap& dict);

}

#endif // DATATYPE_JSON_H
