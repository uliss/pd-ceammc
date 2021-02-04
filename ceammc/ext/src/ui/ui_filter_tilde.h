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
#ifndef UI_FILTER_TILDE_H
#define UI_FILTER_TILDE_H

#include "ceammc_control.h"
#include "ui_filter_t.h"

using namespace ceammc;
using FilterBase = ui::TFilter<UIDspObject>;
using Smoo = control::Smooth<double, control::FloatFix<double>>;

class UIFilterTilde : public FilterBase {
    double z1 { 0 }, z2 { 0 };
    Smoo sm0_, sm1_, sm2_, sm3_, sm4_;

public:
    UIFilterTilde();

    void init(t_symbol* name, const AtomListView& args, bool usePresets);
    void dspOn(double samplerate, long blocksize);
    void dspProcess(t_sample** ins, long n_ins, t_sample** outs, long n_outs, long bs);

    const char* annotateInlet(int) const { return "signal: input"; }
    const char* annotateOutlet(int n) const;

private:
    t_float process(t_float in, double b0, double b1, double b2, double a1, double a2)
    {
        // Transposed direct form II
        double out = in * b0 + z1;
        z1 = in * b1 + z2 - a1 * out;
        z2 = in * b2 - a2 * out;
        return out;
    }
};

void setup_ui_filter_tilde();

#endif // UI_FILTER_TILDE_H
