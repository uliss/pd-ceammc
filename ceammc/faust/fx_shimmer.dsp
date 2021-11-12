declare id "fx.shimmer";

an = library("analyzers.lib");
ba = library("basics.lib");
de = library("delays.lib");
ef = library("misceffects.lib");
fi = library("filters.lib");
ma = library("maths.lib");
os = library("oscillators.lib");
ro = library("routes.lib");
si = library("signals.lib");
ui = library("ceammc_ui.lib");

//------------------------------- par_ps --------------------------------------
// Wariable delay lines based pitch shifter with parametric controller.
// Pitch shifter implementation borrowed from faust examples : pitch_shifter.dsp
// Paramertric conroller represented by oscillator with frequency dependent of
// envelope of input signal.
//
// USAGE:
//   _:par_ps(shift, envelope, control, freq, depth, dry_wet):_
//
// WHERE:
//   shift = frequency shifting (semitones)
//   envelope  = amplitude-envelope time-constant (sec) going down
//   control   = envelope follower to pitch shifter influence coefficient (0..1)
//   freq  = oscillator frequency in Hz
//   depth = oscillator amplitude
//   dry_wet = mix processed and input signals

par_ps(shift, envelope, control2, freq, depth, dry_wet) =
        _<:_,(_<:
        parametric_controller(control2, envelope, freq, depth)*shift,_:
        transpose(c_samples, c_xfade)) : mixer(dry_wet)
with {
    c_samples = 2048;
    c_xfade   = 1024;
    c_folower_colibration = 6;

    parametric_controller(mix, envelope_t, freq, depth) =
        (an.amp_follower(envelope_t):_*c_folower_colibration:
        _*depth,os.osc(freq)*0.5:_,_*depth):mixer(mix):_+0.5;

    transpose (w, x, s, sig)  =
        de.fdelay1s(d,sig)*ma.fmin(d/x,1) + de.fdelay1s(d+w,sig)*(1-ma.fmin(d/x,1))
        with {
            i = 1 - pow(2, s/12);
            d = i : (+ : +(w) : fmod(_,w)) ~ _;
        };

    mixer(mix) = _*(1 - mix),_*mix:>_;
};

//------------------------------- shimmizita_rev_fdn -------------------------------
// Modifiend version of zita FDN reverb. The parametric pitch shifters bank
// embedded into it.
//
// USAGE:
//   bus(8) : shimmizita_rev_fdn(f1,f2,t60dc,t60m, mode, shift,envelope,control,freq,depth,dry_wet, fsmax) : bus(8)
//
// WHERE
//   f1    = crossover frequency (Hz) separating dc and midrange frequencies
//   f2    = frequency (Hz) above f1 where T60 = t60m/2 (see below)
//   t60dc = desired decay time (t60) at frequency 0 (sec)
//   t60m  = desired decay time (t60) at midrange frequencies (sec)
//   mode   = correlation between pitch shift.
//   shift    = --
//   envelope =   |
//   control  =    See par_ps implementation above.
//   freq     =   |
//   depth    =   |
//   dry_wet  = --
//   fsmax = maximum sampling rate to be used (Hz)
//
// REFERENCES from zita:
//   http://www.kokkinizita.net/linuxaudio/zita-rev1-doc/quickguide.html
//   https://ccrma.stanford.edu/~jos/pasp/Zita_Rev1.html

shimmizita_rev_fdn(f1, f2, t60dc, t60m, mode, shift, envelope, control2, freq, depth, dry_wet, fsmax) =
    ((si.bus(2*N) :> allpass_combs(N) : feedbackmatrix(N)) ~
        (delayfilters(N,freqs,durs) : pitchshifters(N) : fbdelaylines(N)))
with {
    N = 8;

    // Delay-line lengths in seconds:
    apdelays = (0.020346, 0.024421, 0.031604, 0.027333, 0.022904,
                0.029291, 0.013458, 0.019123); // feedforward delays in seconds
    tdelays = ( 0.153129, 0.210389, 0.127837, 0.256891, 0.174713,
                0.192303, 0.125000, 0.219991); // total delays in seconds
    tdelay(i) = floor(0.5 + ma.SR * ba.take(i+1,tdelays)); // samples
    apdelay(i) = floor(0.5 + ma.SR * ba.take(i+1,apdelays));
    fbdelay(i) = tdelay(i) - apdelay(i);
    // NOTE: Since SR is not bounded at compile time, we can't use it to
    // allocate delay lines; hence, the fsmax parameter:
    tdelaymaxfs(i) = floor(0.5 + fsmax * ba.take(i+1,tdelays));
    apdelaymaxfs(i) = floor(0.5 + fsmax*ba.take(i+1,apdelays));
    fbdelaymaxfs(i) = tdelaymaxfs(i) - apdelaymaxfs(i);
    nextpow2(x) = ceil(log(x)/log(2.0));
    maxapdelay(i) = int(2.0^max(1.0,nextpow2(apdelaymaxfs(i))));
    maxfbdelay(i) = int(2.0^max(1.0,nextpow2(fbdelaymaxfs(i))));

    apcoeff(i) = select2(i&1,0.6,-0.6);  // allpass comb-filter coefficient
    allpass_combs(N) =
        par(i,N,(fi.allpass_comb(maxapdelay(i),apdelay(i),apcoeff(i))));
    fbdelaylines(N) = par(i,N,(de.delay(maxfbdelay(i),(fbdelay(i)))));

    //Pitch shifters bank
    shiftcoefs_m1 = (1/2,1/3,1/2,1/4,1/2,1/8,1/2,1/3);
    shiftcoefs_m2 = (1/5,-1/2,1/3,-1/2,1/6,-1/3,1/3,1/2);
    shiftdunc(i, shift, mode) = mode<:_>=0,_<0:((_*(shift+shift*mode*ba.take(i+1,shiftcoefs_m1))),(_*(shift+shift*mode*ba.take(i+1,shiftcoefs_m2)))):>_;
    pitchshifters(N) = par(i, N, (_:par_ps(shiftdunc(i, shift, mode), envelope, control2, freq, depth, dry_wet):_));

    freqs = (f1,f2); durs = (t60dc,t60m);
    delayfilters(N,freqs,durs) = par(i,N,filter(i,freqs,durs));
    feedbackmatrix(N) = ro.hadamard(N);

    special_lowpass(g,f) = si.smooth(p) with {
        // unity-dc-gain lowpass needs gain g at frequency f => quadratic formula:
        p = mbo2 - sqrt(max(0,mbo2*mbo2 - 1.0)); // other solution is unstable
        mbo2 = (1.0 - gs*c)/(1.0 - gs); // NOTE: must ensure |g|<1 (t60m finite)
        gs = g*g;
        c = cos(2.0*ma.PI*f/float(ma.SR));
    };

    filter(i,freqs,durs) = lowshelf_lowpass(i)/sqrt(float(N)) + staynormal
    with {
        staynormal = 10.0^(-20); // let signals decay well below LSB, but not to zero

        lowshelf_lowpass(i) = gM*low_shelf1_l(g0/gM,f(1)):special_lowpass(gM,f(2));
        low_shelf1_l(G0,fx,x) = x + (G0-1)*fi.lowpass(1,fx,x); // filter.lib
        g0 = g(0,i);
        gM = g(1,i);
        f(k) = ba.take(k,freqs);
        dur(j) = ba.take(j+1,durs);
        n60(j) = dur(j)*ma.SR; // decay time in samples
        g(j,i) = exp(-3.0*log(10.0)*tdelay(i)/n60(j));
    };
};

process = ba.bypass2(ui.bypass, fx)
with {
    X = ro.cross(2);
    mixer(c,x0,y0,x1,y1) = sel(c,x0,y0), sel(c,x1,y1)
    with {
        sel(c,x,y) = (1-c)*x + c*y;
    };

    rev_fdn = shimmizita_rev_fdn(f1, f2, t60dc, t60m, mode, shift, envelope, control2, speed, depth, ps_dry_wet, fsmax)
    with {
        fsmax = 96000.0;
        f1 = hslider("freq_low [unit:Hz]", 200, 50, 1000, 1);
        t60dc = hslider("decay_low [unit:sec]", 3, 1, 8, 0.1);
        t60m = hslider("decay_mid [unit:sec]", 2, 1, 8, 0.1);
        f2 = hslider("damp_hf [unit:Hz]", 6000, 1500, 0.49*fsmax, 1);
        mode = hslider("mode [type:int]",0, -3, 3, 1);
        shift = hslider("shift [unit:semitone]", 0, -12, +12, 0.1) * 2;
        envelope = hslider("envelope [unit:sec]", 1, 0.1, 3, 0.05);
        control2 = hslider("control", 0.5, 0, 1, 0.05);
        speed = hslider("speed [unit:Hz]", 0.1, 0.1, 10, 0.05);
        depth = hslider("depth", 0, 0, 1, 0.05);
        ps_dry_wet = hslider("ps_drywet", 0.5, 0,1, 0.05);
    };

    fx = _,_<:(_,_<: rev_fdn :>_,_),_,_:_,X,_ : mixer(1 - ui.drywet(0.5));
};
