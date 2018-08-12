#include "list_repeat.h"
#include "../data/datatype_mlist.h"
#include "ceammc_convert.h"
#include "ceammc_dataatomlist.h"
#include "ceammc_factory.h"
#include "ceammc_fn_list.h"

const static int REPEAT_MIN = 0;
const static int REPEAT_MAX = 10000;

ListRepeat::ListRepeat(const ceammc::PdArgs& a)
    : BaseObject(a)
    , times_(0)
{
    createInlet();
    createOutlet();

    times_ = new IntPropertyClosedRange("@times", positionalFloatArgument(0, 1), REPEAT_MIN, REPEAT_MAX);
    createProperty(times_);
}

void ListRepeat::onFloat(float f)
{
    onList(Atom(f));
}

void ListRepeat::onSymbol(t_symbol* s)
{
    onList(Atom(s));
}

void ListRepeat::onList(const AtomList& l)
{
    listTo(0, list::repeat(l, times_->value()));
}

void ListRepeat::onData(const DataPtr& d)
{
    int n = times_->value();

    if (d->type() == DataTypeMList::dataType) {
        DataTypeMList* res = new DataTypeMList;
        auto* src = d->as<DataTypeMList>();

        while (n-- > 0) {
            for (auto& el : *src)
                res->append(el);
        }

        dataTo(0, DataPtr(res));
    } else {
        DataAtomList res;

        while (n-- > 0)
            res.append(d);

        listTo(0, res.toList());
    }
}

void ListRepeat::onInlet(size_t, const AtomList& l)
{
    times_->set(l);
}

void setup_list_repeat()
{
    ObjectFactory<ListRepeat> obj("list.repeat");
    obj.processData();
}
