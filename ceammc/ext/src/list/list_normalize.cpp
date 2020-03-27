#include "list_normalize.h"
#include "ceammc_factory.h"
#include "ceammc_fn_list.h"
#include "ceammc_log.h"
#include "datatype_mlist.h"

static t_symbol* BY_RANGE;
static t_symbol* BY_SUM;

ListNormalize::ListNormalize(const PdArgs& a)
    : ListBase(a)
    , by_(nullptr)
{
    createOutlet();

    by_ = new SymbolEnumProperty("@by", { BY_SUM, BY_RANGE });
    addProperty(by_);

    addProperty(new SymbolEnumAlias("@sum", by_, BY_SUM));
    addProperty(new SymbolEnumAlias("@range", by_, BY_RANGE));
}

void ListNormalize::onList(const AtomList& lst)
{
    AtomList out;

    if (by_->value() == BY_SUM) {
        if (!list::normalizeBySum(lst, out)) {
            OBJ_ERR << "Invalid list values: " << out;
            return;
        }
    } else if (by_->value() == BY_RANGE) {
        if (!list::normalizeByRange(lst, out)) {
            OBJ_ERR << "Invalid list values: " << out;
            return;
        }
    }

    listTo(0, out);
}

void setup_list_normalize()
{
    BY_RANGE = gensym("range");
    BY_SUM = gensym("sum");

    ObjectFactory<ListNormalize> obj("list.normalize");
    obj.processData<DataTypeMList>();
}
