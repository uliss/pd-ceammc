import("stdfaust.lib");
cm = library("ceammc.lib");

vocoder(in) = in : vocoderN(N) : compressor : ma.tanh with {
    N = 36;
    key = 36;
    att = hslider("attack", 10, 0, 500, 0.001) / 1000 : si.smoo;
    rel = hslider("release", 100, 0, 1000, 0.001) / 1000 : si.smoo;
    rolloff = hslider("color", -0.5, -1, 1, 0.001);
    amp = hslider("gain", 0, -60, 6, 0.001) : ba.db2linear : si.smoo;
    excitation = no.colored_noise(5, rolloff) * amp;

    vocoderN(nBands) = _ <: par(i, nBands, vocoder_line(i)) :> _;
    vocoder_line(i) = _ : vocoder_band(i, key, 1) : an.amp_follower_ar(att, rel) : ma.tanh : (_,excitation) : vocoder_band(i, key);
    vocoder_band(band, baseKey, bandGain, x) = x : seq(i, 2, flt) with {
        freq = ba.midikey2hz(baseKey) * pow(2, band/6);
        flt = fi.highpass6e(freq) : fi.lowpass6e(freq) : *(bandGain* 3);
    };

    compressor = co.FFcompressor_N_chan(0.8, -3, 0.001, 0.01, 1, 1, 0, _, 1) : _;
};

FLT_ORDER = 5;
filter_bank_1  = _ <: f1,f2,f3,f4,f5 :> _
with {
    f1 = fi.bandpass(FLT_ORDER, 90, 381);
    f2 = fi.bandpass(FLT_ORDER, 1017, 2713);
    f3 = fi.bandpass(FLT_ORDER, 3223, 3419);
    f4 = fi.bandpass(FLT_ORDER, 4566, 4835);
    f5 = fi.bandpass(FLT_ORDER, 6458, 6838);
};

filter_bank_2 = _ <: f1, f2 :> _
with {
    f1 = fi.bandpass(FLT_ORDER, 89, 300);
    f2 = fi.bandpass(FLT_ORDER, 675, 1012);
};

halaphon_h1(in, ctl) = spat(in) : _,_,_,_
with{
    r = hslider("r", 1, 0, 1, 0.001) : si.smoo;
    att = hslider("attack [unit:ms]", 100, 0, 500, 0.001) / 1000 : si.smoo;
    rel = hslider("release [unit:ms]", 200, 0, 1000, 0.001) / 1000 : si.smoo;
    fast = hslider("fast [unit:Hz]", 0.35, 0.1, 1, 0.001);
    slow = hslider("slow [unit:Hz]", 6, 1, 10, 0.001);
    curve = hslider("curve", -3, -5, 5, 0.001);

    t = ctl : gate : an.amp_follower_ar(att, rel) : conv with {
        thresh = vslider("threshold [unit: db]", -55, -90, 0, 0.1) : si.smoo;
        gate = ef.gate_mono(thresh, att, 0.1, rel);
        conv(in) = ba.if(in <= 0.001, 1000, in : cm.lin2curve(0, 1, slow, fast, curve));
    };
    time = os.lf_sawpos(1/t);
    spat(in) = in : sp.spat(4, time, r);
};

halaphon_h2 = _,_ : spat : _,_,_,_
with{
    spat4(t) = _ : sp.spat(4, t, 1);
    t0 = ma.frac(1.75 - os.lf_sawpos(1/5.0));
    t1 = os.lf_sawpos(1/8.0);
    spat = spat4(t0), spat4(t1) : ro.butterfly(8) : par(i,4,_),par(i,4,!);
};

halaphon_h3 = _,_ : spat : _,_,_,_,_,_
with{
    spat6(t) = _ : sp.spat(6, t, 1) : route(6, 6, ((1,1),(2,2),(3,6),(4,3),(5,4),(6,5)));
    t0 = os.lf_sawpos(1/10.0);
    t1 = ma.frac((5/6) - os.lf_sawpos(1/7.0));
    spat = spat6(t0), spat6(t1) : ro.butterfly(12) : par(i,6,_),par(i,6,!);
};
