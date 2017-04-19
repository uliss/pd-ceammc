#ifndef ARRAY_RANGE_H
#define ARRAY_RANGE_H

#include "array_base.h"

class ArrayRange : public ArrayBase {
public:
    ArrayRange(const PdArgs& a);
    void m_time(t_symbol* s, const AtomList& l);
    void m_sec(t_symbol* s, const AtomList& l);
    void m_ms(t_symbol* s, const AtomList& l);
    void m_sample(t_symbol*, const AtomList& l);
};

#endif // ARRAY_RANGE_H
