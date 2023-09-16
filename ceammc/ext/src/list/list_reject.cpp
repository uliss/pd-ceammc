#include "list_reject.h"
#include "ceammc_containers.h"
#include "ceammc_factory.h"
#include "ceammc_fn_list.h"
#include "datatype_mlist.h"

constexpr size_t BIN_SEARCH_SIZE = 16;

ListReject::ListReject(const PdArgs& args)
    : BaseObject(args)
    , args_(nullptr)
{
    createInlet();
    createOutlet();

    args_ = new ListProperty("@args");
    args_->setArgIndex(0);
    args_->setSuccessFn([this](Property* p) {
        const auto old_size = args_->value().size();
        args_->setValue(list::uniqueSorted(args_->value()));
        if (old_size != args_->value().size())
            OBJ_ERR << "duplicates in reject list, fixed";
    });
    addProperty(args_);
}

void ListReject::onFloat(t_float f)
{
    if (!contains(Atom(f)))
        floatTo(0, f);
}

void ListReject::onSymbol(t_symbol* s)
{
    if (!contains(Atom(s)))
        symbolTo(0, s);
}

void ListReject::onList(const AtomListView& lv)
{
    AtomList32 res;
    res.reserve(lv.size());

    for (auto& a : lv) {
        if (!contains(a))
            res.push_back(a);
    }

    listTo(0, res.view());
}

void ListReject::onInlet(size_t n, const AtomListView& lv)
{
    args_->set(lv);
}

void ListReject::onDataT(const MListAtom& ml)
{
    MListAtom res;
    res->reserve(ml->size());

    for (auto& a : *ml) {
        if (!contains(a))
            res->append(a);
    }

    atomTo(0, res);
}

bool ListReject::contains(const Atom& a) const
{
    auto& l = args_->value();

    if (l.size() >= BIN_SEARCH_SIZE)
        return std::binary_search(l.begin(), l.end(), a);
    else
        return std::find(l.begin(), l.end(), a) != l.end();
}

void setup_list_reject()
{
    ObjectFactory<ListReject> obj("list.reject");
    obj.processData<DataTypeMList>();

    obj.setXletsInfo({ "float, symbol, list or Mlist", "list: set rejected values" }, { "list: without rejected values" });

    obj.setDescription("remove specified element from list");
    obj.setCategory("list");
    obj.setKeywords({"list", "pass", "reject"});
}
