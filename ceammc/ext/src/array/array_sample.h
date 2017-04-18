#ifndef ARRAY_SAMPLE_H
#define ARRAY_SAMPLE_H

#include "ceammc_factory.h"
#include "ceammc_object.h"

using namespace ceammc;

class ArraySample : public BaseObject {
    t_symbol* name_;
    t_garray* array_;

public:
    ArraySample(const PdArgs& a);
    void m_range(t_symbol*, const AtomList& lst);

private:
    t_garray* findArray(t_symbol* s);
};

#endif // ARRAY_SAMPLE_H
