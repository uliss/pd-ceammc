#ifndef ARRAY_SAMPLE_H
#define ARRAY_SAMPLE_H

#include "array_base.h"

class ArraySample : public ArrayBase {
public:
    ArraySample(const PdArgs& a);
    void m_range(t_symbol*, const AtomList& lst);
};

#endif // ARRAY_SAMPLE_H
