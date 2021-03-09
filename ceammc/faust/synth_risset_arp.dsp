declare name "synth.risset_arp";
declare description "Jean Claude Risset's Harmonic Arpeggio Effect";
declare author "Oli Larkin (contact@olilarkin.co.uk)";
declare copyright "Oliver Larkin";
declare version "0.1";
declare licence "GPL";

import("stdfaust.lib");

rissetarp = par(i, N, gen(i)) :> _,_
with {
    gen(idx) = os.osc(thisFreq) : ma.chebychevpoly((1., l(1), l(2), l(3), l(4), l(5), l(6), l(7), l(8))) :  pan(idx)
    with {
        thisFreq = freq + ((idx-4) * fd);
    };

    fd     = hslider("detune", 5., 0., 1000., 0.01) : *(0.01) : si.smoo;
    freq   = hslider("freq", 100, 40, 500, 1) : si.smoo;
    spread = hslider("spread", 1., 0., 1., 0.01) : si.smoo;
    l(i)   = hslider("harmonic%i", 1., 0., 1., 0.01) : si.smoo;
    N = 8;

    pan(s) = _ <: *(v), *(1-v)
    with {
        spreadScale = (1/(N-1));
        v = 0.5 + ((spreadScale * s) - 0.5) * spread;
    };
};

process = rissetarp <: _ * vol, _ * vol with {
    vol = 0.05;
};
