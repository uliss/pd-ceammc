#include "list_at.h"
#include "ceammc_factory.h"
#include "ceammc_log.h"
#include "datatype_mlist.h"

ListAt::ListAt(const PdArgs& a)
    : ListBase(a)
    , default_(nullptr)
{
    static t_symbol* SYM_REL = gensym("rel");
    static t_symbol* SYM_CLIP = gensym("clip");
    static t_symbol* SYM_WRAP = gensym("wrap");
    static t_symbol* SYM_FOLD = gensym("fold");

    createInlet();
    createOutlet();

    pos_ = new ListProperty("@index", AtomList(Atom(0.f)));
    pos_->acceptIntegers();
    pos_->setArgIndex(0);

    addProperty(pos_);

    at_method_ = new SymbolEnumProperty("@method", { SYM_REL, SYM_CLIP, SYM_WRAP, SYM_FOLD });
    addProperty(at_method_);

    addProperty(new SymbolEnumAlias("@rel", at_method_, SYM_REL));
    addProperty(new SymbolEnumAlias("@clip", at_method_, SYM_CLIP));
    addProperty(new SymbolEnumAlias("@wrap", at_method_, SYM_WRAP));
    addProperty(new SymbolEnumAlias("@fold", at_method_, SYM_FOLD));

    default_ = new AtomProperty("@default", Atom());
    addProperty(default_);
}

void ListAt::onInlet(size_t idx, const AtomListView& l)
{
    if (idx != 1)
        return;

    if (!l.allOf(isFloat)) {
        OBJ_ERR << "invalid indexes given: " << l;
        return;
    }

    pos_->set(l);
}

void ListAt::onList(const AtomList& l)
{
    AtomList& p = pos_->value();

    // single item
    if (p.size() == 1) {
        const Atom* a = at(l, p[0]);
        if (!a)
            return;

        atomTo(0, *a);
        return;
    }

    // multiple items
    AtomList res;
    res.reserve(p.size());
    for (size_t i = 0; i < p.size(); i++) {
        const Atom* a = at(l, p[i].asInt(0));
        if (!a)
            continue;

        res.append(*a);
    }

    listTo(0, res);
}

const Atom* ListAt::at(const AtomList& l, const Atom& p)
{
    static t_symbol* SYM_CLIP = gensym("clip");
    static t_symbol* SYM_WRAP = gensym("wrap");
    static t_symbol* SYM_FOLD = gensym("fold");

    const Atom* a = 0;

    if (!p.isInteger()) {
        OBJ_ERR << "invalid index value: " << p;
        return 0;
    }

    int pos = p.asInt(0);

    if (at_method_->value() == SYM_CLIP)
        a = l.clipAt(pos);
    else if (at_method_->value() == SYM_WRAP)
        a = l.wrapAt(pos);
    else if (at_method_->value() == SYM_FOLD)
        a = l.foldAt(pos);
    else
        a = l.relativeAt(pos);

    if (a == 0) {
        if (default_->value().isNone())
            OBJ_ERR << "invalid index value: " << p;
        else
            a = &(default_->value());
    }

    return a;
}

void setup_list_at()
{
    ObjectFactory<ListAt> obj("list.at");
    obj.processData<DataTypeMList>();

    obj.setDescription("outputs list element(s) at specified index(es)");
    obj.addAuthor("Serge Poltavsky");
    obj.setKeywords({ "list" });
    obj.setCategory("list");
    obj.setSinceVersion(0, 1);

    ListAt::setInletsInfo(obj.classPointer(), { "list or Mlist", "set list of indexes" });
    ListAt::setOutletsInfo(obj.classPointer(), { "atom or list" });
}
