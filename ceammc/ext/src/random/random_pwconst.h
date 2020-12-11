#ifndef RANDOM_PWCONST_H
#define RANDOM_PWCONST_H

#include "ceammc_object.h"
#include "rnd_gen.h"
using namespace ceammc;

class RandomPWConst : public BaseObject {
    AtomList values_;
    std::vector<t_float> bounds_;
    std::vector<t_float> weights_;
    RandomGen gen_;
    SizeTProperty* seed_;

public:
    RandomPWConst(const PdArgs& a);
    void onBang() override;
    void onList(const AtomList& v) override;

private:
    bool set(const AtomList& data);
};

void setup_random_pw_const();

#endif // RANDOM_PWCONST_H
