declare name 	"fx.sdelay";
declare author 	"Yann Orlarey";
declare copyright "Grame";
declare version "1.0";
declare license "STK-4.3";

//--------------------------process----------------------------
//
// 	A stereo smooth delay with a feedback control
//
//	This example shows how to use sdelay, a delay that doesn't
//  click and doesn't transpose when the delay time is changed
//-------------------------------------------------------------

import("stdfaust.lib");
cm = library("ceammc.lib");
ui = library("ceammc_ui.lib");

smoothdelay =  (+ :de.sdelay(MAX_SIZE, interp, dtime)) ~ *(fback)
    with
    {
        s = 0.999; // smoothing parameter (one-fi.pole fi.pole location)
        MAX_SIZE = int(ma.SR * 6);
        dtime	= hslider("delay [unit:ms]", 1, 0, 6000, 0.1) * ma.SR / 1000.0 : si.smooth(s);
        fback 	= hslider("feedback", 0, 0, 1, 0.001);
        interp 	= hslider("interpolation [unit:ms]", 20, 1, 200, 0.1) * ma.SR / 1000.0;

    };

process = _ : cm.fx_wrap_mono(ui.bypass, ui.drywet(1), smoothdelay) : _;

