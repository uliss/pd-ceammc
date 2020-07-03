#ifndef RANDOM_DISCRETE_H
#define RANDOM_DISCRETE_H

#include "ceammc_object.h"
using namespace ceammc;

class RandomDiscrete : public BaseObject {
    std::vector<t_float> weights_;

public:
    RandomDiscrete(const PdArgs& a);
    void onBang() override;
    void onList(const AtomList& l) override;

    AtomList propWeights() const;
    void setPropWeights(const AtomList& l);

private:
    bool set(const AtomList& l);
};

void setup_random_discrete();

#endif // RANDOM_DISCRETE_H
