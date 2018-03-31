declare name "DroneBox";
declare description "Mono Sympathetic Resonance Generator";
declare author "Oli Larkin (contact@olilarkin.co.uk)";
declare copyright "Oliver Larkin";
declare version "0.1";
declare licence "GPL";

import("stdfaust.lib");
cm = library("ceammc.lib");
ui = library("ceammc_ui.lib");

smooth_time = 0.005; // secs

coarse_pitch = hslider("pitch [unit:semitones]", 48, 36, 60, 0.001);
t60 = hslider("decay [unit:ms]", 4000, 100, 120000, 1) : cm.time_pd2faust : si.smooth(ba.tau2pole(smooth_time));
mix = ui.drywet(0.5);
damp = 0.3;

dtmax = 4096;
NStrings = 4;

ratios(0) = 1.;
ratios(1) = 1.5;
ratios(2) = 2.;
ratios(3) = 3.;

f = ba.midikey2hz(coarse_pitch) : si.smooth(ba.tau2pole(smooth_time));

string(x, s) = (+ : de.fdelay(dtmax, dtsamples)) ~ (*(fbk))
//string(x, s) = (+ : de.fdelay(dtmax, dtsamples)) ~ (dampingfilter : *(fbk))
with {
	thisFreq = f * ratios(s);
	dtsamples = (ma.SR/thisFreq) - 2;
	fbk = pow(0.001,1.0/(thisFreq*t60));
	h0 = (1. + damp)/2; 
	h1 = (1. - damp)/4;
	dampingfilter(x) = (h0 * x' + h1*(x+x''));
};

dronebox(in) = in <: par(s, NStrings, string(in, s)) :> _;
effect(in) = in <: *(1-mix), (dronebox(in) : *(mix*0.25)) :> _;
process = _ : cm.fx_wrap_mono(ui.bypass, 1, effect) : _;
