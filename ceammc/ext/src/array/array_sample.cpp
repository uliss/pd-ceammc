#include "array_sample.h"

ArraySample::ArraySample(const PdArgs& a)
    : BaseObject(a)
    , name_(0)
    , array_(0)
{
    createOutlet();

    if (!a.args.empty()) {
        const Atom& first = a.args[0];
        if (first.isSymbol())
            name_ = first.asSymbol();

        array_ = findArray(name_);
    }
}

void ArraySample::m_range(t_symbol*, const AtomList& lst)
{
    if (lst.empty() || (!lst[0].isFloat()))
        return;

    if (!name_) {
        OBJ_ERR << "array name is not specified.";
        return;
    }

    // try to find array
    if (!array_) {
        array_ = findArray(name_);
        if (!array_)
            return;
    }

    int vecsize = 0;
    t_word* vecs;
    if (!garray_getfloatwords(array_, &vecsize, &vecs)) {
        OBJ_ERR << "invalid array template: " << name_->s_name;
        return;
    }

    listTo(0, lst * vecsize);
}

t_garray* ArraySample::findArray(t_symbol* s)
{
    if (!s)
        return 0;

    t_garray* arr = reinterpret_cast<t_garray*>(pd_findbyclass(s, garray_class));
    if (!arr) {
        OBJ_ERR << "array not found: " << s->s_name;
        return 0;
    }

    int vecsize = 0;
    t_word* vecs;
    if (!garray_getfloatwords(arr, &vecsize, &vecs)) {
        OBJ_ERR << "invalid array template: " << s->s_name;
        return 0;
    }

    return arr;
}

extern "C" void setup_array0x2esample()
{
    ObjectFactory<ArraySample> obj("array.sample");
    obj.addMethod("range", &ArraySample::m_range);
}
