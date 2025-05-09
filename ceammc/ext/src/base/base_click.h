#ifndef BASE_CLICK_H
#define BASE_CLICK_H

#include "ceammc_sound_external.h"
using namespace ceammc;

class Click : public SoundExternal {
    bool do_click_;

public:
    Click(const PdArgs& a);
    void onBang() final;
    void processBlock(const t_sample** in, t_sample** out) final;

    void click() final;
};

void click_tilde_setup();

#endif // BASE_CLICK_H
