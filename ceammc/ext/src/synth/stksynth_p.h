/*****************************************************************************
 * Copyright 2018 Serge Poltavsky. All rights reserved.
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
#ifndef STKSYNTH_P_H
#define STKSYNTH_P_H

#include "ceammc_property.h"
#include "stksynth.h"

using namespace ceammc;

#include "ADSR.h"
#include "FM.h"
#include "Instrmnt.h"
#include "SineWave.h"

class SinWaveFreq {
    const stk::SineWave* ptr_;

public:
    SinWaveFreq(const stk::SineWave* ptr);
    t_float getFrequency() const;
};

class ADSRProp {
    const stk::ADSR* ptr_;

public:
    ADSRProp(const stk::ADSR* ptr);
    t_float getTarget() const;
};

template <class T>
class StkFMSynth : public T {
public:
    StkFMSynth() try : T() {
    } catch (stk::StkError& e) {
        LIB_ERR << e.getMessage();
        throw e;
    }

    float getControlChange(int ctlNum) const
    {
        switch (ctlNum) {
        case 2:
            return T::control1_ * 64;
        case 4:
            return T::control2_ * 64;
        case 1:
            return (T::modDepth_ / 12) * 128;
        case 11: {
            const stk::SineWave* w = &(T::vibrato_);
            return SinWaveFreq(w).getFrequency();
        }
        case 128:
            return ADSRProp(T::adsr_[1]).getTarget();
        default:
            return -1;
        }
    }

public:
    typedef ControlChangeProperty<StkFMSynth<T>> CCProperty;
};

#endif // STKSYNTH_P_H
