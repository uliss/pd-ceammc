#include "list_length.h"
#include "ceammc_factory.h"
#include "datatype_mlist.h"

ListLength::ListLength(const PdArgs& args)
    : BaseObject(args)
{
    createOutlet();
}

void ListLength::onList(const AtomList& lst)
{
    floatTo(0, lst.size());
}

void ListLength::onDataT(const MListAtom& ml)
{
    floatTo(0, ml->size());
}

static void list_size_fn(PdObject<ListLength>* x, t_symbol*, int argc, t_atom* argv)
{
    if (argc == 1 && Atom::is_data(argv)) {
        Atom a(*argv);
        if (a.isA<DataTypeMList>()) {
            x->impl->onDataT(MListAtom(a));
        } else {
            LIB_ERR << "unsupported data type: " << a.asData()->typeName();
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
