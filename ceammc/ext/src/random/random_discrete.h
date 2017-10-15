#ifndef RANDOM_DISCRETE_H
#define RANDOM_DISCRETE_H

#include "ceammc_dataatomlist.h"
#include "ceammc_object.h"
using namespace ceammc;

class RandomDiscrete : public BaseObject {
    std::vector<t_float> weights_;

public:
    RandomDiscrete(const PdArgs& a);
    void onBang();
    void onList(const AtomList& l);

    AtomList propWeights() const;
    void setPropWeights(const AtomList& l);

private:
    bool set(const AtomList& l);
};

extern "C" void setup_random0x2ediscrete();

#endif // RANDOM_DISCRETE_H
