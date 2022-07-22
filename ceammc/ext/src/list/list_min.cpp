#include "list_min.h"
#include "ceammc_containers.h"
#include "ceammc_crc32.h"
#include "ceammc_factory.h"
#include "datatype_mlist.h"

CEAMMC_DEFINE_SYM_HASH(float);
CEAMMC_DEFINE_SYM_HASH(symbol);
CEAMMC_DEFINE_SYM_HASH(any);

ListMin::ListMin(const PdArgs& a)
    : BaseObject(a)
    , type_(nullptr)
{
    createOutlet();

    type_ = new SymbolEnumProperty("@type", { sym_float(), sym_symbol(), sym_any() });
    addProperty(type_);

    addProperty(new SymbolEnumAlias("@float", type_, sym_float()));
    addProperty(new SymbolEnumAlias("@symbol", type_, sym_symbol()));
    addProperty(new SymbolEnumAlias("@any", type_, sym_any()));
}

void ListMin::onList(const AtomListView& lv)
{
    switch (crc32_hash(type_->value())) {
    case hash_any:
        min(lv.begin(), lv.end());
        break;
    case hash_float:
        min(atom_filter_it_begin(lv, isFloat), atom_filter_it_end(lv));
        break;
    case hash_symbol:
        min(atom_filter_it_begin(lv, isSymbol), atom_filter_it_end(lv));
        break;
    }
}

void ListMin::onDataT(const MListAtom& ml)
{
    onList(ml->data());
}

void setup_list_min()
{
    ObjectFactory<ListMin> obj("list.min");
    obj.useDefaultPdFloatFn();
    obj.useDefaultPdSymbolFn();

    obj.processData<DataTypeMList>();

    obj.setDescription("returns smallest element in the list");
    obj.addAuthor("Serge Poltavsky");
    obj.setKeywords({ "list", "min", "compare" });
    obj.setCategory("list");
    obj.setSinceVersion(0, 1);

    ListMin::setInletsInfo(obj.classPointer(), { "list or Mlist" });
    ListMin::setOutletsInfo(obj.classPointer(), { "atom" });
}
