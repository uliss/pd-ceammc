#ifndef RANDOM_PWCONST_H
#define RANDOM_PWCONST_H

#include "ceammc_object.h"
#include "ceammc_random.h"
using namespace ceammc;

class RandomPWConst : public BaseObject {
    AtomList values_;
    std::vector<t_float> bounds_;
    std::vector<t_float> weights_;
    random::RandomGen gen_;

public:
    RandomPWConst(const PdArgs& a);
    void onBang() override;
    void onList(const AtomListView& lv) override;

private:
    bool set(const AtomListView& data);
};

void setup_random_pw_const();

#endif // RANDOM_PWCONST_H
