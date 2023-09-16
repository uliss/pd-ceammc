declare name 	"fx.delay";
declare author 	"Yann Orlarey";
declare author 	"Serge Poltavski";
declare copyright "Grame";
declare version "1.1";
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

smoothdelay =  (+ : de.sdelay(MAX_SIZE, interp, dtime)) ~(*(fback) : comp : filter)
with
{
    MAX_TIME_SEC = 6;
    MAX_SIZE = int(ma.SR * MAX_TIME_SEC);
    dtime    = hslider("delay [unit:ms]", 1, 0, MAX_TIME_SEC * 1000, 0.1) /(1000) : ba.sec2samp : si.smoo;
    fback    = hslider("feedback", 0, 0, 1, 0.001);
    interp   = hslider("smooth [unit:ms]", 50, 0, 500, 0.1) * 0.001 : ba.sec2samp : int;

    comp = _ : co.FFcompressor_N_chan(strength, thresh, att, rel, 3, 1, 0, _, 1) : _;
    strength = hslider("compress", 0.5, 0, 1, 0.01);
    thresh   = hslider("c_thresh [unit:db]", 0, -60, 0, 0.1);
    att      = hslider("c_attack [unit:ms]", 10, 0.1, 100, 0.1) * 0.001;
    rel      = hslider("c_release [unit:ms]", 50, 1, 500, 0.1) * 0.001;

    filter = _ <: ba.if(checkbox("filter"), fn, _) with {
        fn = fi.lowpass(4, lpf) : fi.highpass(4, hpf);
        hpf = hslider("f_hpf [unit:hz]", 300, 20, 20000, 0.01);
        lpf = hslider("f_lpf [unit:hz]", 9000, 20, 20000, 0.01);
    };
};

process = _ : cm.fx_wrap_mono(ui.bypass, ui.drywet(1), smoothdelay) : _;

