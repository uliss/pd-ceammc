#include "list_route.h"
#include "ceammc_factory.h"
#include "ceammc_format.h"
#include "datatype_mlist.h"

ListRoute::ListRoute(const PdArgs& args)
    : ListBase(args)
    , trim_(nullptr)
    , as_any_(nullptr)
    , args_(nullptr)
{
    createOutlet();

    args_ = new ListProperty("@args");
    args_->setInitOnly();
    args_->setArgIndex(0);
    addProperty(args_);

    trim_ = new FlagProperty("@trim");
    addProperty(trim_);
    as_any_ = new FlagProperty("@as_any");
    addProperty(as_any_);
    simplify_types_ = new BoolProperty("@simplify", true);
    addProperty(simplify_types_);
}

void ListRoute::initDone()
{
    out_annotations_.reserve(args_->value().size() + 1);
    char buf[MAXPDSTRING];

    for (auto& a : args_->value()) {
        createOutlet();
        sprintf(buf, "lists starting with '%s'", atom_gensym(&a.atom())->s_name);
        out_annotations_.push_back(buf);
    }

    out_annotations_.push_back("non-matched values");
}

void ListRoute::onFloat(t_float f)
{
    int idx = args_->value().findPos(Atom(f));

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
    int idx = args_->value().findPos(Atom(s));

    if (idx == -1) {
        return symbolTo(numOutlets() - 1, s);
    } else {
        if (trim_->value())
            return bangTo(idx);
        else {
            if (as_any_->value())
                anyTo(idx, s, AtomListView());
            else
                symbolTo(idx, s);
        }
    }
}

void ListRoute::onList(const AtomListView& lv)
{
    if (lv.empty())
        return;

    int idx = args_->value().findPos(lv.at(0));

    // no match
    if (idx == -1) {
        return listTo(numOutlets() - 1, lv);
    } else {
        if (trim_->value())
            outputList(idx, lv.subView(1));
        else
            outputList(idx, lv);
    }
}

int ListRoute::outletIndex(const Atom& a) const
{
    return args_->value().findPos(a);
}

const char* ListRoute::annotateOutlet(size_t n) const
{
    if (n >= out_annotations_.size())
        return nullptr;

    return out_annotations_[n].c_str();
}

void ListRoute::outputList(size_t idx, const AtomListView& lv)
{
    const size_t sz = lv.size();

    // empty list
    if (sz == 0)
        return bangTo(0);

    // list with first symbol and @as_any flag is set
    if (lv[0].isSymbol() && as_any_->value())
        return anyTo(idx, lv[0].asSymbol(), lv.subView(1));

    if (sz == 1 && simplify_types_->value())
        return atomTo(idx, lv[0]);
    else
        return listTo(idx, lv);
}

void setup_list_route()
{
    ObjectFactory<ListRoute> obj("list.route");
    obj.processData<DataTypeMList>();

    obj.setDescription("acts like [route] but for lists");
    obj.addAuthor("Serge Poltavsky");
    obj.setKeywords({ "list", "route" });
    obj.setCategory("list");
    obj.setSinceVersion(0, 5);

    ListRoute::setInletsInfo(obj.classPointer(), { "list or Mlist" });
}
