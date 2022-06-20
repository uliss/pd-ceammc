#include "list_pass.h"
#include "ceammc_factory.h"

constexpr size_t BIN_SEARCH_SIZE = 20;

ListPass::ListPass(const PdArgs& args)
    : BaseObject(args)
    , args_(nullptr)
{
    createInlet();
    createOutlet();

    args_ = new ListProperty("@args");
    args_->setArgIndex(0);
    args_->setSuccessFn([](Property* p) {
        auto& l = static_cast<ListProperty*>(p)->value();
        if (l.size() >= BIN_SEARCH_SIZE)
            l.sort();
    });
    addProperty(args_);
}

void ListPass::onFloat(t_float f)
{
    if (contains(Atom(f)))
        floatTo(0, f);
}

void ListPass::onSymbol(t_symbol* s)
{
    if (contains(Atom(s)))
        symbolTo(0, s);
}

void ListPass::onList(const AtomList& lst)
{
    AtomList res;
    res.reserve(lst.size());

    for (auto& a : lst) {
        if (contains(a))
            res.append(a);
    }

    listTo(0, res);
}

void ListPass::onInlet(size_t n, const AtomListView& lv) const
{
    args_->set(lv);
}

bool ListPass::contains(const Atom& a) const
{
    auto& l = args_->value();

    if (l.size() >= BIN_SEARCH_SIZE)
        return std::binary_search(l.begin(), l.end(), a);
    else
        return std::find(l.begin(), l.end(), a) != l.end();
}

void setup_list_pass()
{
    ObjectFactory<ListPass> obj("list.pass");
    obj.setXletsInfo({ "float, symbol or list", "list: set passed values" }, { "list: with passed values" });
}
