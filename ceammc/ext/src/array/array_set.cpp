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

void ArraySet::onList(const AtomList& values)
{
    if (values.empty()) {
        OBJ_ERR << "empty list is given";
        return;
    }

    if (normalize_->value()) {
        AtomList norm;
        if (!list::normalizeByRange(values, norm)) {
            OBJ_ERR << "normalization error";
            return;
        }

        set(norm);
    } else
        set(values);
}

void ArraySet::set(const AtomList& v)
{
    if (!checkArray())
        return;

    if (!array_.resize(v.size())) {
        OBJ_ERR << "can't resize array: " << array_.name();
        return;
    }

    for (size_t i = 0; i < v.size(); i++)
        array_[i] = v[i].asFloat();

    array_.redraw();
}

extern "C" void setup_array0x2eset()
{
    ObjectFactory<ArraySet> obj("array.set");
    obj.addAlias("list->array");
}
