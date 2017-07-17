#ifndef RANDOM_PWCONST_H
#define RANDOM_PWCONST_H

#include "ceammc_object.h"
using namespace ceammc;

class RandomPWConst : public BaseObject {
    AtomList values_;
    std::vector<t_float> bounds_;
    std::vector<t_float> weights_;

public:
    RandomPWConst(const PdArgs& a);
    void onBang();
    void onList(const AtomList& v);

    AtomList propBounds() const;
    AtomList propWeights() const;

    AtomList propValues() const;
    void propSetValues(const AtomList& s);

private:
    bool set(const AtomList& data);
};

extern "C" void setup_random0x2epw_const();

#endif // RANDOM_PWCONST_H
