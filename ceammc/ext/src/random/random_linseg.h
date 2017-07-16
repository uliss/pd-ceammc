#ifndef RANDOM_LINSEG_H
#define RANDOM_LINSEG_H

#include "ceammc_object.h"
using namespace ceammc;

class RandomLinSeg : public BaseObject {
    AtomList segs_;
    std::vector<t_float> bounds_;
    std::vector<t_float> weights_;

public:
    RandomLinSeg(const PdArgs& a);
    void onBang();
    void onList(const AtomList& w);

    bool set(const AtomList& data);
    AtomList propSegs() const;
    void propSetSegs(const AtomList& s);
};

extern "C" void setup_random0x2elinseg();

#endif // RANDOM_LINSEG_H
