#include "list_range.h"
#include "ceammc_crc32.h"
#include "ceammc_factory.h"
#include "datatype_mlist.h"

CEAMMC_DEFINE_HASH(float)
CEAMMC_DEFINE_HASH(symbol)
CEAMMC_DEFINE_HASH(any)

crc32_assert_unique(hash_float, hash_symbol, hash_any);

ListRange::ListRange(const PdArgs& a)
    : BaseObject(a)
    , mode_(nullptr)
{
    createOutlet();

    mode_ = new SymbolEnumProperty("@mode", { str_float, str_symbol, str_any });
    addProperty(mode_);

    addProperty(new SymbolEnumAlias("@f", mode_, gensym(str_float)));
    addProperty(new SymbolEnumAlias("@s", mode_, gensym(str_symbol)));
    addProperty(new SymbolEnumAlias("@a", mode_, gensym(str_any)));
}

void ListRange::onList(const AtomList& l)
{
    if (l.empty())
        return;

    Atom min, max;

    const auto hash_mode = crc32_hash(mode_->value());

    for (auto& a : l) {
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
            if (a.isData())
                continue;
            break;
        }

        if (min.isNone() || a < min)
            min = a;

        if (max.isNone() || (max < a))
            max = a;
    }

    if (!min.isNone() && !max.isNone())
        listTo(0, { min, max });
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
