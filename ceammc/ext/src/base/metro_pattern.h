#ifndef METRO_PATTERN_H
#define METRO_PATTERN_H

#include "ceammc_clock.h"
#include "ceammc_object.h"

using namespace ceammc;

class MetroPattern : public BaseObject {
    ClockMemberFunction<MetroPattern> clock_;
    AtomList pattern_;
    AtomList new_pattern_;
    SizeTProperty* current_;
    BoolProperty* sync_;
    bool sync_update_;

public:
    MetroPattern(const PdArgs& args);

    void onFloat(t_float on) override;
    bool p_set_pattern(const AtomList& l);

public:
    void tick();
    t_float currentDelay() const;
    size_t currentIndex() const;
    bool next();

private:
    void output(bool on_start);
};

void setup_metro_pattern();

#endif // METRO_PATTERN_H
