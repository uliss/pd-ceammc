#include "list_repeat.h"
#include "ceammc_dataatomlist.h"
#include "ceammc_factory.h"
#include "ceammc_fn_list.h"

const static int REPEAT_MAX = 10000;

ListRepeat::ListRepeat(const ceammc::PdArgs& a)
    : BaseObject(a)
    , times_(0)
{
    createInlet();
    createOutlet();

    times_ = new IntProperty("@times", int(positionalFloatArgument(0, 1)));
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
    const size_t n = repeatCount();
    if (n == size_t(-1))
        return;

    listTo(0, list::repeat(l, n));
}

void ListRepeat::onData(const AbstractData* d)
{
    size_t n = repeatCount();
    if (n == size_t(-1))
        return;

    DataAtomList res;
    while (n-- > 0) {
        res.append(d);
    }

    listTo(0, res.toList());
}

size_t ListRepeat::repeatCount() const
{
    const int n = times_->value();
    if (n < 0) {
        OBJ_ERR << "invalid repeat value: " << n;
        return size_t(-1);
    }

    if (n > REPEAT_MAX) {
        OBJ_ERR << "too big repeat value: " << n << ". Clip to " << REPEAT_MAX;
        return size_t(REPEAT_MAX);
    }

    return size_t(n);
}

extern "C" void setup_list0x2erepeat()
{
    ObjectFactory<ListRepeat> obj("list.repeat");
    obj.processData();
}
