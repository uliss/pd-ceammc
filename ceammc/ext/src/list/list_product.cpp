#include "list_product.h"
#include "ceammc_factory.h"
#include "datatype_mlist.h"

ListProduct::ListProduct(const PdArgs& args)
    : ListBase(args)
{
    createOutlet();
}

void ListProduct::onFloat(t_float f)
{
    floatTo(0, f);
}

void ListProduct::onList(const AtomListView& lv)
{
    auto p = lv.product();
    if (p == boost::none)
        return;

    floatTo(0, *p);
}

void setup_list_product()
{
    ObjectFactory<ListProduct> obj("list.product");
    obj.processData<DataTypeMList>();

    obj.setDescription("calculates product of floats in list");
    obj.addAuthor("Serge Poltavsky");
    obj.setKeywords({ "list", "product" });
    obj.setCategory("list");
    obj.setSinceVersion(0, 1);

    ListProduct::setInletsInfo(obj.classPointer(), { "list or Mlist" });
    ListProduct::setOutletsInfo(obj.classPointer(), { "float" });
}
