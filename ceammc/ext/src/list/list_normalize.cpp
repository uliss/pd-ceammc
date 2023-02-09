#include "list_normalize.h"
#include "ceammc_crc32.h"
#include "ceammc_factory.h"
#include "ceammc_fn_list.h"
#include "ceammc_log.h"
#include "datatype_mlist.h"

CEAMMC_DEFINE_SYM_HASH(range);
CEAMMC_DEFINE_SYM_HASH(sum)

ListNormalize::ListNormalize(const PdArgs& a)
    : ListBase(a)
    , by_(nullptr)
{
    createOutlet();

    by_ = new SymbolEnumProperty("@by", { sym_sum(), sym_range() });
    addProperty(by_);

    addProperty(new SymbolEnumAlias("@sum", by_, sym_sum()));
    addProperty(new SymbolEnumAlias("@range", by_, sym_range()));
}

void ListNormalize::onList(const AtomListView& lv)
{
    AtomList out;

    switch (crc32_hash(by_->value())) {
    case hash_sum:
        if (!list::normalizeBySum(lv, out)) {
            OBJ_ERR << "Invalid list values: " << out;
            return;
        }
        break;
    case hash_range:
        if (!list::normalizeByRange(lv, out)) {
            OBJ_ERR << "Invalid list values: " << out;
            return;
        }
        break;
    }

    listTo(0, out);
}

void setup_list_normalize()
{
    ObjectFactory<ListNormalize> obj("list.normalize");
    obj.processData<DataTypeMList>();

    obj.setDescription("normalizes float list values to get their sum = 1.0");
    obj.setCategory("list");
}
