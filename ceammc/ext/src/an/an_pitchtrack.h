#ifndef AN_PITCHTRACK_H
#define AN_PITCHTRACK_H

#include "ceammc_clock.h"
#include "ceammc_property_enum.h"
#include "ceammc_sound_external.h"
#include "helmholtz/Helmholtz.h"

#include <memory>

using namespace ceammc;

class PitchTrack : public SoundExternal {
    t_float freq_, fidelity_;
    IntEnumProperty* framesize_;
    IntEnumProperty* overlap_;
    FloatProperty* bias_;
    FloatProperty* maxfreq_;
    FloatProperty* fidelity_threshold_;

    std::unique_ptr<Helmholtz> helmholtz_;
    ClockMemberFunction<PitchTrack> clock_;

public:
    PitchTrack(const PdArgs& args);
    void processBlock(const t_sample** in, t_sample** out) override;

private:
    void initProperties();
    void tick();
};

void setup_an_pitchtrack();

#endif // AN_PITCHTRACK_H
