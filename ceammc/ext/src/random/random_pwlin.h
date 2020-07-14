#ifndef RANDOM_PWLIN_H
#define RANDOM_PWLIN_H

#include "ceammc_object.h"
#include "rnd_gen.h"
using namespace ceammc;

class RandomPwLinear : public BaseObject {
    AtomList values_;
    std::vector<t_float> bounds_;
    std::vector<t_float> weights_;
    RandomGen gen_;
    SizeTProperty* seed_;

public:
    RandomPwLinear(const PdArgs& a);
    void onBang() override;
    void onList(const AtomList& w) override;

    bool set(const AtomList& data);
};

void setup_random_pw_lin();

#endif // RANDOM_PW_LIN_H
