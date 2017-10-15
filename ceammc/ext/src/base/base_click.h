#ifndef BASE_CLICK_H
#define BASE_CLICK_H

#include "ceammc_object.h"
using namespace ceammc;

class Click : public SoundExternal {
    bool do_click_;

public:
    Click(const PdArgs& a);
    void onBang();
    void processBlock(const t_sample** in, t_sample** out);
};

extern "C" void click_tilde_setup();

#endif // BASE_CLICK_H
