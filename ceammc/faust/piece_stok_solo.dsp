import("stdfaust.lib");
declare name "shtokhausen.solo";

maxDelay = 96000 * 45.6 : rint;

scheme = _ : input : output with {
    LOW_CUT = 100;
    HIGH_CUT = 10000;
    fb1_gain = hslider("fb1.gain", 0, 0, 1, 0.0001) : si.smoo;
    fb2_gain = hslider("fb2.gain", 0, 0, 1, 0.0001) : si.smoo;
    fb1_fx(in) = in : *(fb1_gain) : fi.lowpass(3, HIGH_CUT) : fi.highpass(3, LOW_CUT);
    fb2_fx(in) = in : *(fb2_gain) : fi.lowpass(3, HIGH_CUT) : fi.highpass(3, LOW_CUT);

    out1_gain = hslider("out1.gain", 0, 0, 1, 0.0001) * (1-int(checkbox("out1.perf"))) : si.smoo;
    out2_gain = hslider("out2.gain", 0, 0, 1, 0.0001) * (1-int(checkbox("out2.perf"))) : si.smoo;
    output = _*(out1_gain), _*(out2_gain);

    in1_gain = checkbox("in1") : si.smoo;
    in2_gain = checkbox("in2") : si.smoo;

    in1 = _ : *(in1_gain) <: par(i, 6, partN(i) * (checkbox("cycle%i") : si.smoo)) :> _;
    in2 = _ : *(in2_gain) <: par(i, 6, partN(i) * (checkbox("cycle%i") : si.smoo)) :> _;
    partN(i) = de.fdelay(maxDelay, ba.sec2samp(hslider("delay%i[unit:sec]", 0, 0, maxDelay-1, 1)));

    input(in) = (in1 ~+(in, fb1_fx)), (in2 ~+(in, fb2_fx));
};

process = scheme;
