#include "data_list.h"
#include "ceammc_factory.h"
#include "datatype_mlist.h"

DataList::DataList(const PdArgs& a)
    : DataListIFace<BaseObject>(a)
{
    createInlet();
    createOutlet();

    list_ = positionalArguments();
}

void setup_data_list()
{
    ListIFaceFactory<DataList> obj("data.list");
    obj.processData<DataTypeMList>();
}
