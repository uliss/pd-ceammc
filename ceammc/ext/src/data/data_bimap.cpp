/*****************************************************************************
 * Copyright 2023 Serge Poltavski. All rights reserved.
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
#include "data_bimap.h"
#include "ceammc_containers.h"
#include "ceammc_convert.h"
#include "ceammc_factory.h"
#include "ceammc_filesystem.h"
#include "data_protocol.h"
#include "datatype_json.h"
#include "fmt/core.h"

#include "json/json.hpp"

using json = nlohmann::json;

DataBiMap::DataBiMap(const PdArgs& args)
    : DataBiMapFs(args)
{
    createInlet();
    createOutlet();
    createOutlet();

    values_ = new ListProperty("@value");
    values_->setListCheckFn([this](const AtomListView& lv) -> bool { return check(lv); });
    values_->setArgIndex(0);
    values_->setSuccessFn([this](Property* p) { sync(); });
    addProperty(values_);

    createCbIntProperty("@size", [this]() -> int { return bimap_.size(); });
    createCbBoolProperty("@empty", [this]() -> bool { return bimap_.empty(); });
}

void DataBiMap::onFloat(t_float v)
{
    auto it = bimap_.left.find(Atom(v));
    if (it != bimap_.left.end())
        atomTo(0, it->second);
}

void DataBiMap::onSymbol(t_symbol* s)
{
    auto it = bimap_.left.find(Atom(s));
    if (it != bimap_.left.end())
        atomTo(0, it->second);
}

void DataBiMap::onList(const AtomListView& lv)
{
    SmallAtomList res;
    res.reserve(lv.size());

    for (auto& a : lv) {
        auto it = bimap_.left.find(a);
        if (it != bimap_.left.end())
            res.push_back(it->second);
    }

    listTo(0, res.view());
}

void DataBiMap::onInlet(size_t idx, const AtomListView& lv)
{
    if (lv.empty())
        return;
    else if (lv.isAtom()) {
        auto it = bimap_.right.find(lv[0]);
        if (it != bimap_.right.end())
            atomTo(1, it->second);
    } else {
        SmallAtomList res;
        res.reserve(lv.size());

        for (auto& a : lv) {
            auto it = bimap_.right.find(a);
            if (it != bimap_.right.end())
                res.push_back(it->second);
        }

        listTo(1, res.view());
    }
}

bool DataBiMap::proto_write(const std::string& path) const
{
    std::ofstream ofs(path.c_str());
    // can't open
    if (!ofs) {
        LIB_ERR << fmt::format("can not open file: '{}'", path);
        return false;
    }

    try {
        bool first = true;
        ofs << "[\n";
        for (auto& kv : bimap_) {
            if (first) {
                first = false;
            } else
                ofs << ",\n";

            auto& l = kv.left;
            auto& r = kv.right;

            json jl, jr;
            to_json(jl, l);
            to_json(jr, r);

            ofs << "  " << json::array({ jl, jr }).dump();
        }
        ofs << "\n]\n";
        return true;
    } catch (std::exception& e) {
        LIB_ERR << fmt::format("json exception: '{}'", e.what());
        return false;
    }
}

bool DataBiMap::proto_read(const std::string& path)
{
    auto res = fs::readFileContent(path.c_str());
    if (res.isError()) {
        RuntimeError err;
        if (res.matchError(err))
            OBJ_ERR << err.what();

        return false;
    }

    auto& str = res.value();

    try {
        json j = json::parse(str);

        if (j.empty())
            return false;

        if (!j.is_array()) {
            OBJ_ERR << "array expected, got: " << j.type_name();
            return false;
        }

        auto& v = values_->value();
        v.clear();

        for (auto& x : j) {
            if (!x.is_array()) {
                OBJ_ERR << fmt::format("array expected, got: {}, skipping", j.type_name());
                continue;
            }

            if (x.size() == 2 && x.at(0).is_primitive() && x.at(1).is_primitive()) {
                Atom a, b;
                from_json(x.at(0), a);
                v.append(a);

                from_json(x.at(1), b);
                v.append(b);
            } else {
                OBJ_ERR << fmt::format("two privimive values expected, got:  {}", x.dump(0));
            }
        }

        return true;

    } catch (json::exception& e) {
        OBJ_ERR << fmt::format("JSON exception: '{}', while parsing: '{}'", e.what(), str);
        return false;
    }
}

void DataBiMap::editorAddLine(t_symbol* sel, const AtomListView& lv)
{
    if (lv.size() % 2 == 1) {
        OBJ_ERR << "even number of keys expected (list of key1 key2 pairs)";
        return;
    }

    values_->value().append(lv);
    sync();
}

void DataBiMap::editorClear()
{
    values_->value().clear();
    bimap_.clear();
}

int DataBiMap::calcEditorLines() const
{
    return clip<int, 8, 32>(values_->value().size() / 2);
}

int DataBiMap::calcEditorChars() const
{
    return 36;
}

EditorLineList DataBiMap::getContentForEditor() const
{
    EditorLineList res;
    for (size_t i = 0; (i + 1) < values_->value().size(); i += 2) {
        auto str = EditorStringPool::pool().allocate();
        str->str.append(to_string(values_->value()[i]));
        str->str.append(" ");
        str->str.append(to_string(values_->value()[i + 1]));
        res.push_back(str);
    }
    return res;
}

void DataBiMap::m_append(t_symbol* s, const AtomListView& lv)
{
    editorAddLine(s, lv);
}

void DataBiMap::m_clear(t_symbol* s, const AtomListView& lv)
{
    values_->value().clear();
    bimap_.clear();
}

void DataBiMap::m_set(t_symbol* s, const AtomListView& lv)
{
    values_->set(lv);
}

bool DataBiMap::check(const AtomListView& lv) const
{
    return lv.size() > 0 && (lv.size() % 2 == 0);
}

void DataBiMap::sync(bool printErr)
{
    auto& v = values_->value();
    bimap_.clear();

    const auto N = v.size();
    for (size_t i = 0; i < v.size(); i += 2)
        bimap_.insert({ v[i], v[i + 1] });

    v.clear();
    for (auto& bi : bimap_) {
        v.append(bi.get_left());
        v.append(bi.get_right());
    }

    if (N != v.size() && printErr)
        OBJ_ERR << "duplicated keys were skipped: " << v;
}

void setup_data_bimap()
{
    ObjectFactory<DataBiMap> obj("data.bimap");
    DataBiMap::registerMethods(obj);

    obj.addAlias("bimap");
}
