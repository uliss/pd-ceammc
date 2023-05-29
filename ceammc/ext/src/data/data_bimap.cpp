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
#include "datatype_json.h"
#include "fmt/core.h"

#include "json/json.hpp"

using json = nlohmann::json;

DataBiMapBase::DataBiMapBase(const PdArgs& args)
    : EditorObject<FilesystemIFace<BaseObject>>(args)
{
    createInlet();
    createOutlet();
    createOutlet();

    auto v = createCbListProperty(
        "@value", [this]() { return list(); },
        [this](const AtomList& ls) -> bool { list() = ls; return true; });

    v->setListCheckFn([this](const AtomListView& lv) -> bool { return check(lv); });
    v->setSuccessFn([this](Property* p) { sync(); });

    createCbIntProperty("@size", [this]() -> int { return bimap().size(); });
    createCbBoolProperty("@empty", [this]() -> bool { return bimap().empty(); });

    createCbListProperty("@keys_left", [this]() {
        AtomList res;
        res.reserve(bimap().size());

        for (auto& kv : bimap().left)
            res.append(kv.first);

        return res;
    });

    createCbListProperty("@keys_right", [this]() {
        AtomList res;
        res.reserve(bimap().size());

        for (auto& kv : bimap().right)
            res.append(kv.first);

        return res;
    });
}

void DataBiMapBase::onFloat(t_float v)
{
    auto it = bimap().left.find(Atom(v));
    if (it != bimap().left.end())
        atomTo(0, it->second);
}

void DataBiMapBase::onSymbol(t_symbol* s)
{
    auto it = bimap().left.find(Atom(s));
    if (it != bimap().left.end())
        atomTo(0, it->second);
}

void DataBiMapBase::onList(const AtomListView& lv)
{
    SmallAtomList res;
    res.reserve(lv.size());

    for (auto& a : lv) {
        auto it = bimap().left.find(a);
        if (it != bimap().left.end())
            res.push_back(it->second);
    }

    listTo(0, res.view());
}

void DataBiMapBase::onInlet(size_t idx, const AtomListView& lv)
{
    if (lv.empty())
        return;
    else if (lv.isAtom()) {
        auto it = bimap().right.find(lv[0]);
        if (it != bimap().right.end())
            atomTo(1, it->second);
    } else {
        SmallAtomList res;
        res.reserve(lv.size());

        for (auto& a : lv) {
            auto it = bimap().right.find(a);
            if (it != bimap().right.end())
                res.push_back(it->second);
        }

        listTo(1, res.view());
    }
}

bool DataBiMapBase::proto_write(const std::string& path) const
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
        for (auto& kv : bimap()) {
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

bool DataBiMapBase::proto_read(const std::string& path)
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

        list().clear();

        for (auto& x : j) {
            if (!x.is_array()) {
                OBJ_ERR << fmt::format("array expected, got: {}, skipping", j.type_name());
                continue;
            }

            if (x.size() == 2 && x.at(0).is_primitive() && x.at(1).is_primitive()) {
                Atom a, b;
                from_json(x.at(0), a);
                list().append(a);

                from_json(x.at(1), b);
                list().append(b);
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

void DataBiMapBase::editorAddLine(t_symbol* sel, const AtomListView& lv)
{
    if (lv.size() % 2 == 1) {
        OBJ_ERR << "even number of keys expected (list of key1 key2 pairs)";
        return;
    }

    list().append(lv);
    sync();
}

void DataBiMapBase::editorClear()
{
    list().clear();
    bimap().clear();
}

int DataBiMapBase::calcEditorLines() const
{
    return clip<int, 8, 32>(list().size() / 2);
}

int DataBiMapBase::calcEditorChars() const
{
    return 36;
}

EditorLineList DataBiMapBase::getContentForEditor() const
{
    EditorLineList res;
    for (size_t i = 0; (i + 1) < list().size(); i += 2) {
        auto str = EditorStringPool::pool().allocate();
        str->str.append(to_string(list()[i]));
        str->str.append(" ");
        str->str.append(to_string(list()[i + 1]));
        res.push_back(str);
    }
    return res;
}

void DataBiMapBase::m_insert(t_symbol* s, const AtomListView& lv)
{
    editorAddLine(s, lv);
}

void DataBiMapBase::m_clear(t_symbol* s, const AtomListView& lv)
{
    list().clear();
    bimap().clear();
}

void DataBiMapBase::m_set(t_symbol* s, const AtomListView& lv)
{
    setProperty(s, lv);
}

const char* DataBiMapBase::annotateInlet(size_t n) const
{
    return (n == 0) ? "atom: left key" : "atom: right key";
}

const char* DataBiMapBase::annotateOutlet(size_t n) const
{
    return (n == 0) ? "atom: right value" : "atom: left value";
}

bool DataBiMapBase::check(const AtomListView& lv) const
{
    return lv.size() > 0 && (lv.size() % 2 == 0);
}

void DataBiMapBase::sync(bool printErr)
{
    auto& v = list();
    bimap().clear();

    const auto N = v.size();
    for (size_t i = 0; i < v.size(); i += 2)
        bimap().insert({ v[i], v[i + 1] });

    v.clear();
    for (auto& bi : bimap()) {
        v.append(bi.get_left());
        v.append(bi.get_right());
    }

    if (N != v.size() && printErr)
        OBJ_ERR << "duplicated keys were skipped: " << v;
}

void DataBiMapBase::setArgIndex(std::int8_t idx)
{
    auto pv = property("@value");
    if (pv)
        pv->setArgIndex(idx);
}

DataBiMap::DataBiMap(const PdArgs& args)
    : DataBiMapBase(args)
{
    setArgIndex(0);
}

void setup_data_bimap()
{
    ObjectFactory<DataBiMap> obj("data.bimap");
    DataBiMap::registerMethods(obj);

    obj.addAlias("bimap");
}
