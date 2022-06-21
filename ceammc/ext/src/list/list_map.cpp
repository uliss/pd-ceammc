#include "list_map.h"
#include "ceammc_factory.h"
#include "ceammc_format.h"
#include "ceammc_numeric.h"
#include "datatype_dict.h"
#include "fmt/format.h"

ListMap::ListMap(const PdArgs& args)
    : BaseObject(args)
{
    auto& d = parsedPosArgs();
    if (d.isA<DataTypeDict>())
        dict_ = DictAtom(d[0]);
    else
        OBJ_ERR << "dict expected, got: " << d;

    createInlet();
    createOutlet();
}

void ListMap::onFloat(t_float f)
{
    if (!math::is_integer(f)) {
        OBJ_ERR << fmt::format("only integer or symbol keys are supported, got: {}", f);
        return;
    }

    onSymbol(gensym(fmt::format("{}", long(f)).c_str()));
}

void ListMap::onSymbol(t_symbol* s)
{
    if (!dict_) {
        OBJ_ERR << "empty dict";
        return;
    }

    auto it = dict_->find(s);
    if (it == dict_->end())
        return;

    listTo(0, it->second);
}

void ListMap::onList(const AtomList& lst)
{
    if (!dict_) {
        OBJ_ERR << "empty dict";
        return;
    }

    AtomList res;
    res.reserve(lst.size());
    auto& d = *dict_;
    auto end_it = d.end();

    for (auto& a : lst) {
        t_symbol* key = nullptr;
        if (a.isSymbol())
            key = a.asT<t_symbol*>();
        else if (a.isInteger())
            key = gensym(fmt::format("{}", a.asT<int>()).c_str());
        else {
            OBJ_ERR << fmt::format("skipping atom {} - only symbol or integer keys are allowed", to_string(a));
            continue;
        }

        auto it = d.find(key);
        if (it != end_it)
            res.append(it->second);
    }

    listTo(0, res);
}

void ListMap::onInlet(size_t n, const AtomListView& lv)
{
}

void setup_list_map()
{
    ObjectFactory<ListMap> obj("list.map");
}
