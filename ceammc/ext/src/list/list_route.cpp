#include "list_route.h"
#include "datatype_mlist.h"
#include "ceammc_factory.h"

ListRoute::ListRoute(const PdArgs& args)
    : ListBase(args)
    , trim_(0)
    , as_any_(0)
{
    createOutlet();

    for (size_t i = 0; i < positionalArguments().size(); i++)
        createOutlet();

    trim_ = new FlagProperty("@trim");
    addProperty(trim_);
    as_any_ = new FlagProperty("@as_any");
    addProperty(as_any_);
    simplify_types_ = new BoolProperty("@simplify", true);
    addProperty(simplify_types_);
}

void ListRoute::onFloat(t_float f)
{
    int idx = positionalArguments().findPos(Atom(f));

    if (idx == -1) {
        return floatTo(numOutlets() - 1, f);
    } else {
        if (trim_->value())
            bangTo(idx);
        else
            floatTo(idx, f);
    }
}

void ListRoute::onSymbol(t_symbol* s)
{
    int idx = positionalArguments().findPos(Atom(s));

    if (idx == -1) {
        return symbolTo(numOutlets() - 1, s);
    } else {
        if (trim_->value())
            return bangTo(idx);
        else {
            if (as_any_->value())
                anyTo(idx, s, AtomList());
            else
                symbolTo(idx, s);
        }
    }
}

void ListRoute::onList(const AtomList& lst)
{
    if (lst.empty())
        return;

    int idx = positionalArguments().findPos(lst.at(0));

    // no match
    if (idx == -1) {
        return listTo(numOutlets() - 1, lst);
    } else {
        if (trim_->value())
            outputList(idx, lst.slice(1));
        else
            outputList(idx, lst);
    }
}

int ListRoute::outletIndex(const Atom& a) const
{
    return positionalArguments().findPos(a);
}

void ListRoute::outputList(size_t idx, const AtomList& l)
{
    const size_t sz = l.size();

    // empty list
    if (sz == 0)
        return bangTo(0);

    // list with first symbol and @as_any flag is set
    if (l[0].isSymbol() && as_any_->value())
        return anyTo(idx, l[0].asSymbol(), l.slice(1));

    if (sz == 1 && simplify_types_->value())
        return atomTo(idx, l[0]);
    else
        return listTo(idx, l);
}

void setup_list_route()
{
    ObjectFactory<ListRoute> obj("list.route");
    obj.processData<DataTypeMList>();
}
