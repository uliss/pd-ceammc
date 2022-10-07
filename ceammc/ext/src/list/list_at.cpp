#include "list_at.h"
#include "ceammc_containers.h"
#include "ceammc_crc32.h"
#include "ceammc_factory.h"
#include "ceammc_log.h"
#include "datatype_mlist.h"

CEAMMC_DEFINE_SYM_HASH(rel);
CEAMMC_DEFINE_SYM_HASH(clip);
CEAMMC_DEFINE_SYM_HASH(wrap);
CEAMMC_DEFINE_SYM_HASH(fold);

ListAt::ListAt(const PdArgs& a)
    : ListBase(a)
    , default_(nullptr)
{
    createInlet();
    createOutlet();

    pos_ = new ListProperty("@index", AtomList(Atom(0.f)));
    pos_->acceptIntegers();
    pos_->setArgIndex(0);

    addProperty(pos_);

    at_method_ = new SymbolEnumProperty("@method", { sym_rel(), sym_clip(), sym_wrap(), sym_fold() });
    addProperty(at_method_);

    addProperty(new SymbolEnumAlias("@rel", at_method_, sym_rel()));
    addProperty(new SymbolEnumAlias("@clip", at_method_, sym_clip()));
    addProperty(new SymbolEnumAlias("@wrap", at_method_, sym_wrap()));
    addProperty(new SymbolEnumAlias("@fold", at_method_, sym_fold()));

    default_ = new AtomProperty("@default", Atom());
    addProperty(default_);
}

void ListAt::onInlet(size_t idx, const AtomListView& lv)
{
    if (idx != 1)
        return;

    if (!lv.allOf(isFloat)) {
        OBJ_ERR << "invalid indexes given: " << lv;
        return;
    }

    pos_->set(lv);
}

void ListAt::onList(const AtomListView& lv)
{
    AtomList& p = pos_->value();

    // single item
    if (p.size() == 1) {
        const Atom* a = at(lv, p[0]);
        if (!a)
            return;

        return atomTo(0, *a);
    }

    // multiple items
    AtomList16 res;
    res.reserve(p.size());
    for (size_t i = 0; i < p.size(); i++) {
        const Atom* a = at(lv, p[i].asInt(0));
        if (!a)
            continue;

        res.push_back(*a);
    }

    listTo(0, res.view());
}

const Atom* ListAt::at(const AtomListView& lv, const Atom& p)
{
    const Atom* a = nullptr;

    if (!p.isInteger()) {
        OBJ_ERR << "invalid index value: " << p;
        return 0;
    }

    int pos = p.asInt(0);

    switch (crc32_hash(at_method_->value())) {
    case hash_clip:
        a = lv.clipAt(pos);
        break;
    case hash_wrap:
        a = lv.wrapAt(pos);
        break;
    case hash_fold:
        a = lv.foldAt(pos);
        break;
    default:
        a = lv.relativeAt(pos);
        break;
    }

    if (a == nullptr) {
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
