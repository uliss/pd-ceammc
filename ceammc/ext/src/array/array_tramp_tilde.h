/*****************************************************************************
 * Copyright 2023 Serge Poltavski. All rights reserved.
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
#ifndef ARRAY_TRAMP_TILDE_H
#define ARRAY_TRAMP_TILDE_H

#include "array_base.h"
#include "ceammc_clock.h"

class ArrayTRampTilde : public ArraySoundBase {
    FloatProperty* speed_;
    FloatProperty* from_;
    FloatProperty* to_;
    ClockLambdaFunction clock_;
    double phase_;
    bool is_running_;

public:
    ArrayTRampTilde(const PdArgs& args);

    void onBang() override;

    void setupDSP(t_signal** sp) override;
    void processBlock(const t_sample** in, t_sample** out) final;

private:
    size_t rampStart() const;
    size_t fromSamples() const { return std::round(from_->value() * (array_.size() - 1)); }
    size_t toSamples() const { return std::round(to_->value() * (array_.size() - 1)); }
};

void setup_array_tramp_tilde();

#endif // ARRAY_TRAMP_TILDE_H
