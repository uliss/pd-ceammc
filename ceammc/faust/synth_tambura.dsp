declare name "Tambura";
declare description "Pseudo physical model of an Indian Tambura/Tanpura";
declare author "Oli Larkin (contact@olilarkin.co.uk)";
declare copyright "Oliver Larkin";
declare version "1.0";
declare licence "GPL";

//TODO
// - pitch env doesn't get triggered by autoplucker
// - autoplucker fixed to 4 strings

import("stdfaust.lib");
cm = library("ceammc.lib");

line (value, time) = state~(_,_):!,_
	with {
		state (t, c) = nt, ba.if (nt <= 0, value, c+(value - c) / nt)
		with {
			nt = ba.if( value != value', samples, t-1);
			samples = time*ma.SR/1000.0;
		};
	};

dtmax = 4096;

//tunings of the four strings, ratios of f0
ratios(0) = 1.5;
ratios(1) = 2.;
ratios(2) = 2.01;
ratios(3) = 1.;

NStrings = 4;

sm = si.smooth(ba.tau2pole(0.05)); //50 ms smoothing

//ratios(i) = hslider("ratio%1i", 1., 0.1, 2., 0.001);
pluck(i)  = button("pluck%1i"); // buttons for manual plucking
pluckrate = hslider("auto_pluck_rate [unit:hz]", 0.1, 0.0, 2, 0.001); // automatic plucking rate (Hz)
enableautoplucker = checkbox("auto_pluck"); // enable automatic plucking

f0       = hslider("pitch", 36, 24, 72, 1) : sm : ba.midikey2hz; // the base pitch of the drone
t60      = hslider("decay_time [unit:ms]", 10000, 0, 100000, 0.1) : cm.time_pd2faust : sm; // how long the strings decay
damp     = 1. - hslider("high_freq_loss", 0, 0, 1., 0.01) : sm; // string brightness
fd       = hslider("harmonic_motion [scale:exp]", 0.001, 0., 1, 0.0001) : *(0.2) : sm; // controls the detuning of parallel waveguides that mimics harmonic motion of the tambura
coupling = hslider("sympathetic_coupling", 0.1, 0., 1., 0.0001) : sm; // level of sympathetic coupling between strings
jw       = hslider("jawari", 0, 0, 1, 0.001) : *(0.1) : sm; // creates the buzzing / jawari effect
spread   = hslider("string_spread", 1., 0., 1., 0.01) : sm; // stereo spread of strings

tscale  = hslider("tune_scale", 1, 0.9, 1.1, 0.001); //
descale = hslider("decay_scale", 1, 0.1, 1., 0.001); //
//dascale = hslider("/h:main/[10]damp_scale", 1, 0.5, 2, 0.01); //

ptype = hslider("material", 0.13, 0.0, 1., 0.01) : sm; // crossfades between pink noise and DC excitation
pattack = hslider("attack_time [scale:exp]", 0.07, 0, 0.5, 0.01); // attack time of pluck envelope, 0 to 0.5 times f0 wavelength
ptime = hslider("decay_time", 1., 1, 100., 0.01); // decay time (1 to 10 times f0 wavelength)
ppos = hslider("position", 0.25, 0.01, 0.5, 0.01); // pick position (ratio of f0 wavelength)
pbend = hslider("bend_depth [unit:st]", 3, 0., 12., 0.01); // pick bend depth in semitones
pbendtime = hslider("bend_time [unit:ms]", 10., 1, 200., 1); // pick bend time (1 to 200 ms)

// s = string index
// c = comb filter index (of 9 comb filters in risset string)
tambura(NStrings) = ( couplingmatrix(NStrings), par(s, NStrings, excitation(s)) : ro.interleave(NStrings, 2) : par(s, NStrings, string(s, pluck(s))) ) // string itself with excitation + fbk as input
                    ~ par(s, NStrings, (!,_)) // feedback only the right waveguide
                    : par(s, NStrings, (+:pan(s)) // add left/right waveguides and pan
                    ) :> _,_ //stereo output
 with {

    couplingmatrix(NStrings) = 
      par(s, NStrings, *(coupling) : couplingfilter) // coupling filters
      <: par(s, NStrings, unsel(NStrings, s) :> _ ) // unsel makes sure the feedback is disconnected

      with {
          unsel(NStrings,s) = par(j, NStrings, U(s,j))
          with {
            U(s,s)=!;
            U(s,j)=_;
          };
          
          couplingfilter = fi.highshelf(1, -100, 5000) : fi.peak_eq(14, 2500, 400) : fi.peak_eq(20, 7500, 650); // EQ to simulate bridge response
    };

    pan(s) = _ <: *((1-v) : sqrt), *((v) : sqrt)
    with {
      spreadScale = (1/(NStrings-1));
      v = 0.5 + ((spreadScale * s) - 0.5) * spread;
    };

//    excitation(s) = _;
    excitation(s, trig) = input * ampenv : pickposfilter
      with {
        wl = (ma.SR/(f0 * ratios(s))); // wavelength of f0 in samples
        dur = (ptime * wl) / (ma.SR/1000.); // duration of the pluck in ms
        ampenv = trig * line(1. - trig, dur) : si.lag_ud(wl * pattack * (1/ma.SR), 0.005);
        amprand = abs(no.noise) : ba.latch(trig) *(0.25) + (0.75);
        posrand = abs(no.noise) : ba.latch(trig) *(0.2);
        input = 1., no.pink_noise : si.interpolate(ptype); // crossfade between DC and pink noise excitation source
        pickposfilter = fi.ffcombfilter(dtmax, ((ppos + posrand) * wl), -1); // simulation of different pluck positions
      };

    string(s, trig) = _, _ <: +, !,_ : rissetstring(_, s, 1., 1., 1.), rissetstring(_, s, tscale, descale, 1.) // dual risset strings for decoupled feedback
    with {
      rissetstring(x, s, ts, des, das) = _ <: par(c, 9, stringloop(x, s, c, ts, das)) :> _ : fi.dcblocker *(0.01); // 9 detuned delay line resonators in parallel
      stringloop(x, s, c, ts, des, das) = (+ : delay) ~ ((dampingfilter : nlfm) * fbk) // waveguide string with damping filter and non linear apf for jawari effect
      with {
        //delay = de.fdelay1a(dtmax, dtsamples, x); // allpass interpolation has better HF response
        delay = de.fdelaylti(2, dtmax, dtsamples, x); // lagrange interpolation glitches less with pitch envelope
        pitchenv = trig * line(1. - trig, pbendtime) <: * : *(pbend);
        thisf0 = ba.pianokey2hz( ba.hz2pianokey((f0 * ratios(s)) + ((c-4) * fd) + pitchenv) ) * ts;
        dtsamples = (ma.SR/thisf0) - 2;
        fbk = pow(0.001, 1.0/(thisf0*(t60 * descale)));
        dampingfilter(x) = (h0 * x' + h1*(x+x''))
        with {
          d = das * damp;
          h0 = (1. + d)/2;
          h1 = (1. - d)/4;
        };
        nlfm(x) = x <: fi.allpassnn(1,(par(i,1,jw * ma.PI * x)));
      };
    };
};

autoplucker= phasor(pluckrate) <: <(0.25), >(0.25) & <(0.5), >(0.5) & <(0.75), >(0.75) & <(1) : par(s, NStrings, *(enableautoplucker))
with {
  phasor(freq) = (freq/float(ma.SR) : (+ : ma.decimal) ~ _);
};

process = (par(s, NStrings, pluck(s)), autoplucker) :> tambura(NStrings) :> _;
