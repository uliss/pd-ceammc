#include "list_range.h"
#include "ceammc_containers.h"
#include "ceammc_crc32.h"
#include "ceammc_factory.h"
#include "datatype_mlist.h"

CEAMMC_DEFINE_SYM_HASH(float)
CEAMMC_DEFINE_SYM_HASH(symbol)
CEAMMC_DEFINE_SYM_HASH(any)

crc32_assert_unique(hash_float, hash_symbol, hash_any);

ListRange::ListRange(const PdArgs& a)
    : BaseObject(a)
    , type_(nullptr)
{
    createOutlet();

    type_ = new SymbolEnumProperty("@type", { str_float, str_symbol, str_any });
    addProperty(type_);

    addProperty(new SymbolEnumAlias("@f", type_, sym_float()));
    addProperty(new SymbolEnumAlias("@s", type_, sym_symbol()));
    addProperty(new SymbolEnumAlias("@a", type_, sym_any()));
}

void ListRange::onList(const AtomListView& lv)
{
    if (lv.empty())
        return;

    Atom min, max;

    const auto hash_mode = crc32_hash(type_->value());

    for (auto& a : lv) {
        if (a.isData())
            continue;

        switch (hash_mode) {
        case hash_float:
            if (!a.isFloat())
                continue;
            break;
        case hash_symbol:
            if (!a.isSymbol())
                continue;
            break;
        default:
            break;
        }

        if (min.isNone() || a < min)
            min = a;

        if (max.isNone() || (max < a))
            max = a;
    }

    if (!min.isNone() && !max.isNone()) {
        StaticAtomList<2> msg { min, max };
        listTo(0, msg.view());
    }
}

void ListRange::onDataT(const MListAtom& ml)
{
    onList(ml->data());
}

void setup_list_range()
{
    ObjectFactory<ListRange> obj("list.range");
    obj.addAlias("list.minmax");
    obj.processData<DataTypeMList>();

    obj.setDescription("output list smallest and largest value");
    obj.addAuthor("Serge Poltavsky");
    obj.setKeywords({ "list", "range", "compare" });
    obj.setCategory("list");
    obj.setSinceVersion(0, 1);

    ListRange::setInletsInfo(obj.classPointer(), { "list or Mlist" });
    ListRange::setOutletsInfo(obj.classPointer(), { "list: min max" });
}
