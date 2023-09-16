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

    void onFloat(t_float f) override;
    void onInlet(size_t n, const AtomListView& lv) override;

    void clockTick();

private:
    bool setPattern(const AtomListView& lv);
};

void setup_metro_seq();

#endif // METRO_SEQ_H
