#include "list_max.h"
#include "ceammc_containers.h"
#include "ceammc_crc32.h"
#include "ceammc_factory.h"
#include "datatype_mlist.h"

CEAMMC_DEFINE_SYM_HASH(float);
CEAMMC_DEFINE_SYM_HASH(symbol);
CEAMMC_DEFINE_SYM_HASH(any);

ListMax::ListMax(const PdArgs& a)
    : BaseObject(a)
    , type_(0)
{
    createOutlet();

    type_ = new SymbolEnumProperty("@type", { sym_float(), sym_symbol(), sym_any() });
    addProperty(type_);

    addProperty(new SymbolEnumAlias("@float", type_, sym_float()));
    addProperty(new SymbolEnumAlias("@symbol", type_, sym_symbol()));
    addProperty(new SymbolEnumAlias("@any", type_, sym_any()));
}

void ListMax::onList(const AtomListView& lv)
{
    switch (crc32_hash(type_->value())) {
    case hash_any:
        max(lv.begin(), lv.end());
        break;
    case hash_float:
        max(atom_filter_it_begin(lv, isFloat), atom_filter_it_end(lv));
        break;
    case hash_symbol:
        max(atom_filter_it_begin(lv, isSymbol), atom_filter_it_end(lv));
        break;
    }
}

void ListMax::onDataT(const MListAtom& ml)
{
    onList(ml->data());
}

void setup_list_max()
{
    ObjectFactory<ListMax> obj("list.max");
    obj.useDefaultPdFloatFn();
    obj.useDefaultPdSymbolFn();

    obj.processData<DataTypeMList>();

    obj.setDescription("output largest element in the list");
    obj.addAuthor("Serge Poltavsky");
    obj.setKeywords({ "list", "max", "compare" });
    obj.setCategory("list");
    obj.setSinceVersion(0, 1);

    ListMax::setInletsInfo(obj.classPointer(), { "list or Mlist" });
    ListMax::setOutletsInfo(obj.classPointer(), { "atom" });
}
