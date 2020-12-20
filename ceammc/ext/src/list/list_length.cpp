#include "list_length.h"
#include "ceammc_factory.h"
#include "datatype_mlist.h"

ListLength::ListLength(const PdArgs& args)
    : BaseObject(args)
{
    createOutlet();
}

static void list_size_fn(PdObject<ListLength>* x, t_symbol*, int argc, t_atom* argv)
{
    if (argc == 1 && Atom::is_data(argv)) {
        Atom a(*argv);
        if (a.isA<DataTypeMList>())
            x->impl->floatTo(0, a.asD<DataTypeMList>()->size());
        else
            LIB_ERR << "unsupported data type: " << a.asData()->typeName();
    } else
        x->impl->floatTo(0, argc);
}

void setup_list_length()
{
    ObjectFactory<ListLength> obj("list.length");
    obj.addAlias("list.size");
    obj.setListFn(list_size_fn);
    obj.setFlag(OBJECT_FACTORY_NO_LIST, true);

    obj.setDescription("output the number of atoms in the list");
    obj.addAuthor("Serge Poltavsky");
    obj.setKeywords({ "list", "length", "size" });
    obj.setCategory("list");
    obj.setSinceVersion(0, 1);

    ListLength::setInletsInfo(obj.classPointer(), { "list or Mlist" });
    ListLength::setOutletsInfo(obj.classPointer(), { "int" });
}
