/*****************************************************************************
 * Copyright 2021 Serge Poltavsky. All rights reserved.
 *
 * This file may be distributed under the terms of GNU Public License version
 * 3 (GPL v3) as defined by the Free Software Foundation (FSF). A copy of the
 * license should have been included with this file, or the project in which
 * this file belongs to. You may also find the details of GPL v3 at:
 * http://www.gnu.org/licenses/gpl-3.0.txt
 *
 * If you have any questions regarding the use of this file, feel free to
 * contact the author of this file, or the owner of the project in which
 * this file belongs to.
 *****************************************************************************/
#ifndef AUBIO_PITCHSHIFT_TILDE_H
#define AUBIO_PITCHSHIFT_TILDE_H

#include <map>
#include <memory>

#include "aubio/src/aubio.h"
#include "ceammc_property_enum.h"
#include "ceammc_sound_external.h"
using namespace ceammc;

extern "C" {
typedef struct _aubio_pitchshift_t aubio_pitchshift_t;
}

class AubioPitchshiftTilde : public SoundExternal {
public:
    enum OptionType {
        PHASE,
        SMOOTH,
        QUALITY,
        WINDOW
    };

private:
    std::unique_ptr<aubio_pitchshift_t, void (*)(aubio_pitchshift_t*)> impl_;
    SymbolEnumProperty* window_;
    SymbolEnumProperty* quality_;
    BoolProperty* smooth_;
    BoolProperty* phase_;
    std::vector<smpl_t> in_, out_;
    t_float scale_, transpose_;
    std::map<OptionType, std::string> options_;
    std::string opt_string_;

public:
    AubioPitchshiftTilde(const PdArgs& args);
    ~AubioPitchshiftTilde();

    void setupDSP(t_signal** sig) override;
    void processBlock(const t_sample** in, t_sample** out) override;

    void onInlet(size_t n, const AtomListView& lv) override;
    void update();
};

void setup_aubio_pitchshift_tilde();

#endif // AUBIO_PITCHSHIFT_TILDE_H
