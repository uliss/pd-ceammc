#include "list_length.h"
#include "datatype_mlist.h"
#include "ceammc_factory.h"

ListLength::ListLength(const PdArgs& args)
    : BaseObject(args)
{
    createOutlet();
}

void ListLength::onList(const AtomList& lst)
{
    floatTo(0, lst.size());
}

void ListLength::onDataT(const DataTypeMList& lst)
{
    floatTo(0, lst.size());
}

static void list_size_fn(PdObject<ListLength>* x, t_symbol*, int argc, t_atom* argv)
{
    if (argc == 1 && Atom(*argv).isData()) {
        Atom data(*argv);
        DataTPtr<DataTypeMList> ptr(data);
        if (ptr.isValid()) {
            x->impl->onDataT(*ptr.data());
        } else {
            DataDesc d = data.getData();
            LIB_ERR << "can't get data with type=" << d.type << " and id=" << d.id;
        }
    } else {
        x->impl->floatTo(0, argc);
    }
}

void setup_list_length()
{
    ObjectFactory<ListLength> obj("list.length");
    obj.addAlias("list.size");
    obj.setListFn(list_size_fn);
}
