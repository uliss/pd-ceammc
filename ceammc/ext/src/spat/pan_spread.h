#ifndef PAN_SPREAD_H
#define PAN_SPREAD_H

#include "ceammc_signal.h"
#include "ceammc_sound_external.h"

using namespace ceammc;

class PanSpread : public SoundExternal {
    InitIntPropertyClosedRange* channels_;
    BoolProperty* compensate_;
    std::vector<t_float> positions_;
    std::vector<t_float> block_l_;
    std::vector<t_float> block_r_;
    std::vector<t_float> coefs_l_;
    std::vector<t_float> coefs_r_;
    SmoothLinT<t_float> spread_;
    SmoothLinT<t_float> center_;

public:
    PanSpread(const PdArgs& args);
    void setupDSP(t_signal** sp);
    void processBlock(const t_sample** in, t_sample** out);

    void calcCoefficents();
    AtomList propCoeffs() const;

    AtomList propSpread() const;
    void propSetSpread(const AtomList& lst);
    AtomList propCenter() const;
    void propSetCenter(const AtomList& lst);
};

void setup_pan_spread();

#endif // PAN_SPREAD_H
