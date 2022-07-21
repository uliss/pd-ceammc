#include "array_set.h"
#include "ceammc_factory.h"
#include "ceammc_fn_list.h"

ArraySet::ArraySet(const PdArgs& a)
    : ArrayMod(a)
    , normalize_(0)
{
    normalize_ = new FlagProperty("@normalize");
    addProperty(normalize_);
}

void ArraySet::onSymbol(t_symbol* s)
{
    setArray(s);
}

void ArraySet::onList(const AtomListView& lv)
{
    if (lv.empty()) {
        OBJ_ERR << "empty list is given";
        return;
    }

    if (normalize_->value()) {
        AtomList norm;
        if (!list::normalizeByRange(lv, norm)) {
            OBJ_ERR << "normalization error";
            return;
        }

        set(norm.view());
    } else
        set(lv);
}

void ArraySet::set(const AtomListView& lv)
{
    if (!checkArray())
        return;

    if (!array_.resize(lv.size())) {
        OBJ_ERR << "can't resize array: " << array_.name();
        return;
    }

    for (size_t i = 0; i < lv.size(); i++)
        array_[i] = lv[i].asFloat();

    array_.redraw();
}

void setup_array_set()
{
    ObjectFactory<ArraySet> obj("array.set");
    obj.addAlias("list->array");
}
