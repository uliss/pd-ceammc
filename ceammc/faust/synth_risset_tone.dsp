declare name "Risset Tone";
declare description "Jean Claude Risset's endless glissando";
declare author "Oli Larkin (contact@olilarkin.co.uk)";
declare copyright "Oliver Larkin";
declare version "0.1";
declare licence "GPL";

import("stdfaust.lib");

rate = hslider("rate", 0.01, -2, 2, 0.001);
centroid = hslider("centroid", 60, 20, 120, 1);
range = hslider("range", 120, 1, 120, 1) / 2.;
Npartials = 10;
tblsize = 1 << 16;

thin_env = exp(-4.8283*(1.-cos((2.0*ma.PI)*(float(ba.time) - (tblsize/2.))/tblsize)));
wide_env = 0.5 + 5. * cos((2.0*ma.PI)*((float(ba.time)-(tblsize/2.))/tblsize)) / 10.;

lookup(phase, env)=ss1+d*(ss2-ss1)
with {
  idx = int(phase * tblsize);
  d = ma.decimal(phase * tblsize);
  ss1 = rdtable(tblsize+1,env,idx);
  ss2 = rdtable(tblsize+1,env,idx+1);
};

rissetTone(freq, N) = phasor(freq) <: par(i, N, modphase(_, i) : partial) :> _
with {
  phasor(freq) = (freq/float(ma.SR) : (+ : ma.decimal) ~ _);

  modphase(x, i) = fmod(x+phaseDiff, 1.)
  with {
    phaseDiff = (1. / float(N)) * i;
  };

  partial(pos) = pos : *(2.) : -(1.) : *(range) : +(centroid) : +(7) : ba.midikey2hz : os.osc * lookup(pos, thin_env);
};

process = rissetTone(rate, Npartials) : *(1/Npartials);
