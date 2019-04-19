declare name 		"fx.distortion1";
declare version 	"0.01";
declare author 		"brummer";
declare license 	"BSD";
declare copyright 	"(c)brummer 2008";

import("stdfaust.lib");
import("guitarix.lib");
import("maxmsp.lib");

F = nentry("split_low_freq [unit:Hz]", 250, 20, 600, 10);
F1 = nentry("split_middle_freq [unit:Hz]", 650, 600, 1250, 10);
F2 = nentry("split_high_freq [unit:Hz]", 1250, 1250, 12000, 10);

//------------------------------ analyzer --------------------------------------
analyzern(O,lfreqs) = _ <: bsplit(nb) with
{
   nb = ba.count(lfreqs);
   fc(n) = ba.take(n, lfreqs);
   lp(n) = fi.lowpass(O,fc(n));
   hp(n) = fi.highpass(O,fc(n));
   bsplit(0) = _;
   bsplit(i) = hp(i), (lp(i) <: bsplit(i-1));
};

analyzerN(lfreqs) = analyzern(3,lfreqs);

//----------distortion---------

//-speaker emulation
sbp1    		= vslider("sp_low_freq [unit:Hz]",130,20,1000,10);
sbp2    		= vslider("sp_high_freq [unit:Hz]",5000,1000,12000,10);
switch1 		= checkbox("sp_on_off");
sbp 			= hgroup("low_highcutoff", ba.bypass1(switch1, +(anti_denormal_ac) : ef.speakerbp(sbp1, sbp2)));

//-low and highpass
lowpassfreq  	= nentry("lp_freq [unit:Hz]", 5000, 20, 12000, 10);
highpassfreq 	= nentry("hp_freq [unit:Hz]", 130, 20, 7040, 10);
switch       	= checkbox("flt_on_off");
passo 		 	= +(anti_denormal_ac) : fi.lowpass(1,lowpassfreq) : fi.highpass(1,highpassfreq);
pass 		 	= hgroup("low_highpass", ba.bypass1(switch, passo));

//-distortion
drivelevel      = vslider("level", 0.01, 0, 0.5, 0.01);
drivegain1      = vslider("gain [unit:db]", 2, -10, 10, 0.1)-10 : ba.db2linear : smoothi(0.999);
low_gain      	= vslider("low_gain [unit:db]", 10, -10, 20, 0.1)-10 : ba.db2linear : smoothi(0.999);
high_gain      	= vslider("high_gain [unit:db]", 10, -10, 20, 0.1)-10 : ba.db2linear : smoothi(0.999);
middle_gain_l   = vslider("middle_l_gain [unit:db]", 10, -10, 20, 0.1)-10 : ba.db2linear : smoothi(0.999);
middle_gain_h   = vslider("middle_h_gain [unit:db]", 10, -10, 20, 0.1)-10 : ba.db2linear : smoothi(0.999);
drive			= vslider("drive", 0.64, 0, 1, 0.01);
drive1			= vslider("low_drive", 1, 0, 1, 0.01) * drive;
drive2			= vslider("high_drive", 1, 0, 1, 0.01) * drive;
drive3			= vslider("middle_l_drive", 1, 0, 1, 0.01) * drive;
drive4			= vslider("middle_h_drive", 1, 0, 1, 0.01) * drive;
distortion1 	=  _ : ef.cubicnl(drive1, drivelevel) : *(low_gain);
distortion2 	=  _ : ef.cubicnl(drive2, drivelevel) : *(high_gain);
distortion3 	=  _ : ef.cubicnl(drive3, drivelevel) : *(middle_gain_l);
distortion4 	=  _ : ef.cubicnl(drive4, drivelevel) : *(middle_gain_h);
distortion	= _ : analyzerN((F,(F1,F2))) : distortion2,distortion4 ,distortion3,distortion1 :>_;

//-resonator
resonator 		= (+ <: (de.delay(4096, d-1) + de.delay(4096, d)) / 2) ~ *(1.0-a)
with {
  d = vslider("vibrato", 1, 0, 1, 0.01);
  a = vslider("trigger", 0.12, 0, 1, 0.01);
};

switch2       	= checkbox("res_on_off");

hs 				= component("HighShelf.dsp").hs;
process 		= bypass(switch2, resonator) : +(anti_denormal_ac) : pass  : sbp : hs : distortion : *(drivegain1) : hs : sbp;
