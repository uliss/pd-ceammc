#ifndef RANDOM_DISCRETE_H
#define RANDOM_DISCRETE_H

#include "ceammc_object.h"
#include "rnd_gen.h"
using namespace ceammc;

class RandomDiscrete : public BaseObject {
    std::vector<t_float> weights_;
    RandomGen gen_;
    SizeTProperty* seed_;

public:
    RandomDiscrete(const PdArgs& a);
    void onBang() override;
    void onList(const AtomListView& lv) override;

private:
    bool set(const AtomListView& lv);
};

void setup_random_discrete();

#endif // RANDOM_DISCRETE_H
