#include "list_product.h"
#include "datatype_mlist.h"
#include "ceammc_factory.h"

ListProduct::ListProduct(const PdArgs& args)
    : BaseObject(args)
{
    createOutlet();
}

void ListProduct::onFloat(t_float f)
{
    floatTo(0, f);
}

void ListProduct::onList(const AtomList& lst)
{
    auto p = lst.product();
    if (p == boost::none)
        return;

    floatTo(0, *p);
}

void ListProduct::onDataT(const DataTypeMList& lst)
{
    onList(lst.toList());
}

void setup_list_product()
{
    ObjectFactory<ListProduct> obj("list.product");
    obj.processData<DataTypeMList>();
}
