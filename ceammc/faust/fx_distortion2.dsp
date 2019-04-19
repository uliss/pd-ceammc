declare name 		"fx.distortion2";
declare version 	"0.01";
declare author 		"brummer";
declare license 	"BSD";
declare copyright 	"(c)brummer 2008";

import("stdfaust.lib");
import("guitarix.lib");

//----------distortion---------

//-speaker emulation
sbp1    = vslider("sp_low_freq [unit:Hz]", 130, 20, 1000, 10);
sbp2    = vslider("sp_high_freq [unit:Hz]", 5000, 1000, 12000, 10);
switch1 = checkbox("sp_on_off");
sbp     = hgroup("low_highcutoff", ba.bypass1(switch1, +(anti_denormal_ac) : ef.speakerbp(sbp1, sbp2)));

//-low and highpass
lowpassfreq  = nentry("low_freq[unit:Hz]", 5000, 20, 12000, 10);
highpassfreq = nentry("high_freq[unit:Hz]", 130, 20, 7040, 10);
switch       = checkbox("flt_on_off");
passo        = +(anti_denormal_ac) : fi.lowpass(1, lowpassfreq) : fi.highpass(1, highpassfreq );
pass         = hgroup("low_highpass", ba.bypass1(switch, passo));

//-distortion
drivelevel   = vslider("level", 0.01, 0, 0.5, 0.01);
drivegain1   = vslider("gain [unit:db]", 2, -10, 10, 0.1)-10 : ba.db2linear : si.smoo;
drive        = vslider("drive", 0.64, 0, 1, 0.01);
distortion   = ef.cubicnl(drive, drivelevel);

//-resonator
switch2      = checkbox("res_on_off");
resonator = (+ <: (de.delay(4096, d-1) + de.delay(4096, d)) / 2) ~ *(1.0-a)
with {
    d = vslider("vibrato", 1, 0, 1, 0.01);
    a = vslider("trigger", 0.12, 0, 1, 0.01);
};

process = bypass(switch2, resonator) : +(anti_denormal_ac) : pass : add_dc : sbp
    : component("HighShelf.dsp").hs : distortion : *(drivegain1) : component("HighShelf.dsp").hs : sbp;
