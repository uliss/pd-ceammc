#include "list_normalize.h"
#include "ceammc_factory.h"
#include "ceammc_fn_list.h"
#include "ceammc_log.h"
#include "datatype_mlist.h"

static t_symbol* BY_RANGE;
static t_symbol* BY_SUM;

ListNormalize::ListNormalize(const PdArgs& a)
    : BaseObject(a)
    , by_(0)
{
    createOutlet();

    by_ = new SymbolEnumProperty("@by", BY_SUM);
    by_->appendEnum(BY_RANGE);
    createProperty(by_);

    createProperty(new SymbolEnumAlias("@sum", by_, BY_SUM));
    createProperty(new SymbolEnumAlias("@range", by_, BY_RANGE));
}

void ListNormalize::onList(const AtomList& lst)
{
    AtomList out(lst.filtered(isFloat));

    if (by_->value() == BY_SUM) {
        if (!out.normalizeFloats()) {
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

void ListNormalize::onDataT(const DataTypeMList& lst)
{
    onList(lst.toList());
}

void setup_list_normalize()
{
    BY_RANGE = gensym("range");
    BY_SUM = gensym("sum");

    ObjectFactory<ListNormalize> obj("list.normalize");
    obj.processData<DataTypeMList>();
}
