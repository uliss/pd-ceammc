#include "array_sample.h"

ArraySample::ArraySample(const PdArgs& a)
    : ArrayBase(a)
{
    createOutlet();
}

void ArraySample::m_range(t_symbol*, const AtomList& lst)
{
    if (lst.empty() || (!lst[0].isFloat()))
        return;

    int array_size = arraySize();
    if (array_size < 0)
        return;

    listTo(0, lst * array_size);
}

extern "C" void setup_array0x2esample()
{
    ObjectFactory<ArraySample> obj("array.sample");
    obj.addMethod("range", &ArraySample::m_range);
}
