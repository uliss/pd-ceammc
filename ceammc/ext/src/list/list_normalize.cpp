#include "list_normalize.h"
#include "ceammc_factory.h"
#include "ceammc_fn_list.h"
#include "ceammc_log.h"

static t_symbol* BY_RANGE = gensym("range");
static t_symbol* BY_SUM = gensym("sum");

ListNormalize::ListNormalize(const PdArgs& a)
    : BaseObject(a)
    , by_(0)
{
    createOutlet();

    by_ = new SymbolEnumProperty("@by", "sum");
    by_->appendEnum("range");
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

extern "C" void setup_list0x2enormalize()
{
    ObjectFactory<ListNormalize> obj("list.normalize");
}
