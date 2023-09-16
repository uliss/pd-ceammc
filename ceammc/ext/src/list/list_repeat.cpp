#include "list_repeat.h"
#include "ceammc_factory.h"
#include "ceammc_fn_list.h"
#include "datatype_mlist.h"

constexpr int REPEAT_DEF = 1;
constexpr int REPEAT_MIN = 0;
constexpr int REPEAT_MAX = 10000;

ListRepeat::ListRepeat(const ceammc::PdArgs& a)
    : BaseObject(a)
    , times_(0)
{
    createInlet();
    createOutlet();

    times_ = new IntProperty("@n", REPEAT_DEF);
    times_->setArgIndex(0);
    times_->checkClosedRange(REPEAT_MIN, REPEAT_MAX);
    addProperty(times_);
}

void ListRepeat::onFloat(t_float f)
{
    onList(Atom(f));
}

void ListRepeat::onSymbol(t_symbol* s)
{
    onList(Atom(s));
}

void ListRepeat::onList(const AtomListView& lv)
{
    listTo(0, list::repeat(lv, times_->value()));
}

void ListRepeat::onData(const Atom& d)
{
    int n = times_->value();

    if (d.isA<DataTypeMList>()) {
        MListAtom res;
        auto* src = d.asD<DataTypeMList>();

        while (n-- > 0)
            res->append(src->data());

        atomTo(0, res);
    } else {
        AtomList res;

        while (n-- > 0)
            res.append(d);

        listTo(0, res);
    }
}

void ListRepeat::onInlet(size_t, const AtomListView& l)
{
    times_->set(l);
}

void setup_list_repeat()
{
    ObjectFactory<ListRepeat> obj("list.repeat");
    obj.processData();

    obj.setDescription("output new list by repeating input list specified times");
    obj.addAuthor("Serge Poltavsky");
    obj.setKeywords({ "list", "repeat" });
    obj.setCategory("list");
    obj.setSinceVersion(0, 1);

    ListRepeat::setInletsInfo(obj.classPointer(), { "list or Mlist", "int: set number of repeats" });
    ListRepeat::setOutletsInfo(obj.classPointer(), { "list or Mlist" });
}
