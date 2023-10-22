#include "list_map.h"
#include "ceammc_containers.h"
#include "ceammc_factory.h"
#include "ceammc_format.h"
#include "ceammc_numeric.h"
#include "datatype_dict.h"
#include "datatype_mlist.h"
#include "fmt/core.h"

ListMap::ListMap(const PdArgs& args)
    : BaseObject(args)
{
    createCbListProperty(
        "@dict",
        [this]() -> AtomList { return dict_; },
        [this](const AtomListView& lv) -> bool {
            if (lv.isA<DataTypeDict>()) {
                dict_ = DictAtom(lv[0]);
                return true;
            } else if (!dict_->setFromDataList(lv)) {
                OBJ_ERR << "dict expected, got: " << lv;
                return false;
            }

            return true;
        })
        ->setArgIndex(0);

    def_ = new ListProperty("@def");
    addProperty(def_);

    createInlet();
    createOutlet();
    createOutlet();
}

void ListMap::onFloat(t_float f)
{
    if (!math::is_integer<t_float>(f)) {
        OBJ_ERR << fmt::format("invalid key: '{}', only integer or symbol keys are supported", f);
        return;
    }

    onSymbol(gensym(fmt::format("{}", long(f)).c_str()));
}

void ListMap::onSymbol(t_symbol* s)
{
    if (!dict_) {
        OBJ_ERR << "NULL dict";
        return;
    }

    auto it = dict_->find(s);
    if (it == dict_->end()) {

        if (def_->value().empty())
            return atomTo(1, s);
        else
            return listTo(0, def_->value());
    }

    listTo(0, it->second);
}

void ListMap::onList(const AtomListView& lv)
{
    if (!dict_) {
        OBJ_ERR << "NULL dict";
        return;
    }

    SmallAtomListN<32> res;
    res.reserve(lv.size());

    auto& d = *dict_;
    auto end_it = d.end();

    for (auto& a : lv) {
        t_symbol* key = nullptr;
        if (a.isSymbol())
            key = a.asT<t_symbol*>();
        else if (a.isInteger())
            key = gensym(fmt::format("{}", a.asT<int>()).c_str());
        else {
            OBJ_ERR << fmt::format("skipping key '{}', only symbol or integer keys are allowed", to_string(a));
            continue;
        }

        auto it = d.find(key);
        if (it != end_it) {
            res.insert_back(it->second);
        } else if (def_->value().size() > 0) {
            for (auto& a : def_->value())
                res.push_back(a);
        } else {
            symbolTo(1, key);
        }
    }

    listTo(0, res.view());
}

void ListMap::onInlet(size_t n, const AtomListView& lv)
{
    if (lv.isA<DataTypeDict>())
        dict_ = DictAtom(lv[0]);
    else if (!dict_->setFromDataList(lv))
        OBJ_ERR << fmt::format("Dict expected, got: {}", to_string(lv));
}

void ListMap::onDataT(const MListAtom& ma)
{
    MListAtom res;
    auto& d = *dict_;
    auto end_it = d.end();

    for (auto& a : *ma) {
        t_symbol* key = nullptr;
        if (a.isSymbol())
            key = a.asT<t_symbol*>();
        else if (a.isInteger())
            key = gensym(fmt::format("{}", a.asT<int>()).c_str());
        else {
            OBJ_ERR << fmt::format("skipping key '{}', only symbol or integer keys are allowed", to_string(a));
            continue;
        }

        auto it = d.find(key);
        if (it != end_it) {
            res->append(it->second);
        } else if (def_->value().size() > 0) {
            for (auto& a : def_->value())
                res->append(a);
        } else {
            symbolTo(1, key);
        }
    }

    atomTo(0, res);
}

void setup_list_map()
{
    ObjectFactory<ListMap> obj("list.map");
    obj.processData<DataTypeMList>();

    obj.setXletsInfo({ "float, symbol, list or Mlist", "dict: set mapping data" }, { "list or Mlist", "atom: non-matched value" });

    obj.setDescription("map list values by specified dictionary");
    obj.setCategory("list");
    obj.setKeywords({ "list", "map", "dict" });
}
