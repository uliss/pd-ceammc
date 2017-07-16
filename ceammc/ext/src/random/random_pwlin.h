#ifndef RANDOM_PWLIN_H
#define RANDOM_PWLIN_H

#include "ceammc_object.h"
using namespace ceammc;

class RandomPwLinear : public BaseObject {
    AtomList segs_;
    std::vector<t_float> bounds_;
    std::vector<t_float> weights_;

public:
    RandomPwLinear(const PdArgs& a);
    void onBang();
    void onList(const AtomList& w);

    bool set(const AtomList& data);
    AtomList propSegs() const;
    void propSetSegs(const AtomList& s);
};

extern "C" void setup_random0x2epw_lin();

#endif // RANDOM_PW_LIN_H
