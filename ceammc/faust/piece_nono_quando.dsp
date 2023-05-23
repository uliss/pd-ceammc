import("stdfaust.lib");

declare name "nono.quando";

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

halafon(N,t,r) = sp.spat(N,time,r) : halmeters(N)
  with{
    time = os.lf_sawpos(1/t);
    h1(v) = vgroup("Ch %v", hmeter);
    halmeters(N) = vgroup("h1 meters", par(i, N, h1(i)));
    hmeter(x) = attach(x, an.amp_follower(0.150, x) : ba.linear2db : hbargraph("[05][unit:dB] Meter", -70, +5));
};
