#ifndef METRO_PATTERN_H
#define METRO_PATTERN_H

#include "ceammc_clock.h"
#include "ceammc_object.h"

using namespace ceammc;

class MetroPattern : public BaseObject {
    ClockMemberFunction<MetroPattern> clock_;
    AtomList pattern_;
    SizeTProperty* current_;

public:
    MetroPattern(const PdArgs& args);

    void onFloat(t_float on);
    AtomList p_pattern() const;
    void p_set_pattern(const AtomList& l);

public:
    void tick();
    t_float currentDelay() const;
    bool next();
};

void setup_metro_pattern();

#endif // METRO_PATTERN_H
