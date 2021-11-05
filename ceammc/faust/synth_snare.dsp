declare name "synth.snare";

import("stdfaust.lib");
cm = library("ceammc.lib");

// Port from SuperCollider to Faust of snare_stein in
// SynthDefPool by Dan Stowell, which in turn was based
// on a Sound-on-Sound 'synth secrets' tutorial
// ceammc: from guitarix
// license: GPL2+

//https://github.com/josmithiii/faust-jos/tree/master/percussion

decay(n,x) = x - (x>0.0)/n;
release(n) = + ~ decay(n);
envgate(dur,trigger) = trigger : release(int(dur*float(ma.SR))) : >(0.0);

adsr(a,d,s,r,t) = env ~ (_,_) : (!,_) // the 2 'state' signals are fed back
with {
    env (p2,y) =
        (t>0) & (p2|(y>=1)),          // p2 = decay-sustain phase
        (y + p1*u - (p2&(y>s))*v*y - p3*w*y)	// y  = envelop signal
	*((p3==0)|(y>=eps)) // cut off tails to prevent denormals
    with {
		p1 = (p2==0) & (t>0) & (y<1);         // p1 = attack phase
		p3 = (t<=0) & (y>0);                  // p3 = release phase
		// #samples in attack, decay, release, must be >0
		na = ma.SR*a+(a==0.0); nd = ma.SR*d+(d==0.0); nr = ma.SR*r+(r==0.0);
		// correct zero sustain level
		z = s+(s==0.0)*ba.db2linear(-60);
		// attack, decay and (-60dB) release rates
		u = 1/na; v = 1-pow(z, 1/nd); w = 1-1/pow(z*ba.db2linear(60), 1/nr);
		// values below this threshold are considered zero in the release phase
		eps = ba.db2linear(-120);
    };
};

process = snare * amp with {
    gate = checkbox("gate [type:float]");
    trigger = (gate>gate');

    // using gate value as amplitude: sample and hold it on gate open
    amp = gate  : cm.clip(0, 1) : ba.latch(trigger) : *(ba.db2linear(-12));

    lpf(freq) = fi.lowpass(3,freq);
    hpf(freq) = fi.highpass(3,freq);

    lpnoise = no.noise : lpf(7040);
    hpnoise = no.noise : hpf(523);
    // att = 0.0005; // attack-time in seconds
    att = vslider("attack [unit:ms]", 0.5, 0.3, 100, 0.01) : cm.time_pd2faust;
    dec = vslider("decay [unit:ms]", 1, 1, 100, 0.01) : cm.time_pd2faust;
    release = vslider("release [unit:ms]", 200, 10, 1000, 0.01);
    rel_nlo = release : cm.time_pd2faust;
    rel_nhi = rel_nlo * 0.915;
    rel_shi = rel_nlo * 0.275;
    rel_slo = rel_nlo * 0.375;

    perc(att,rel,trigger) = adsr(att,dec,1.0,rel,envgate(att,trigger));
    sinosc0(freq) = os.oscrs(freq); // SinOsc at phase fi.zero

    snare = (0.25 + sinosc0(330)) * perc(att,rel_shi,trigger)
          + (0.25 + sinosc0(185)) * perc(att,rel_slo,trigger)
          + 0.2 * lpnoise * perc(att,rel_nlo,trigger)
          + 0.2 * hpnoise * perc(att,rel_nhi,trigger);
};
