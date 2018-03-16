#ifndef METRO_SEQ_H
#define METRO_SEQ_H

#include "ceammc_clock.h"
#include "ceammc_object.h"

using namespace ceammc;

class MetroSeq : public BaseObject {
    ClockMemberFunction<MetroSeq> clock_;
    AtomList pattern_;
    FloatProperty* interval_;
    size_t current_;

public:
    MetroSeq(const PdArgs& a);

    void onFloat(t_float f);
    void clockTick();

public:
    AtomList p_current() const;
    void p_set_current(const AtomList& l);
    AtomList p_pattern() const;
    void p_set_pattern(const AtomList& l);
};

void setup_metro_seq();

#endif // METRO_SEQ_H
