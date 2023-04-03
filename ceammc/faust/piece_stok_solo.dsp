import("stdfaust.lib");
declare name "shtokhausen.solo";

maxDelay = 96000 * 45.6 : rint;

scheme = _,_ : input : output with {
    smooth_amount = hslider("smooth", 10, 0, 100, 0.01) * 0.00999;
    smoo = si.smooth(smooth_amount);

    LOW_CUT = 100;
    HIGH_CUT = 10000;
    fb_filter = fi.lowpass(3, HIGH_CUT) : fi.highpass(3, LOW_CUT);
    fb_gate = ef.gate_mono(hslider("threshhold[unit:db]", -40, -60, 0, 0.01), 0.0001, 0.200, 0.100);
    fb_compress = co.limiter_1176_R4_mono;
    fb_post_chain = fb_gate : fb_filter : fb_compress;


    fb1_gain = hslider("fb1.gain", 0, 0, 1, 0.0001) : smoo;
    fb2_gain = hslider("fb2.gain", 0, 0, 1, 0.0001) : smoo;
    fb1_fx(in) = in : *(fb1_gain) : fb_post_chain;
    fb2_fx(in) = in : *(fb2_gain) : fb_post_chain;

    out1_gain = hslider("out1.gain", 0, 0, 1, 0.0001) : smoo;
    out2_gain = hslider("out2.gain", 0, 0, 1, 0.0001) : smoo;
    output = _*(out1_gain), _*(out2_gain) : co.limiter_1176_R4_stereo;

    in1_gain = checkbox("in1") : smoo;
    in2_gain = checkbox("in2") : smoo;

    in1 = _ : *(in1_gain) <: par(i, 6, partN(i) * (checkbox("cycle%i") : si.smoo)) :> _;
    in2 = _ : *(in2_gain) <: par(i, 6, partN(i) * (checkbox("cycle%i") : si.smoo)) :> _;
    partN(i) = de.fdelay(maxDelay, ba.sec2samp(hslider("delay%i[unit:sec]", 0, 0, maxDelay-1, 1)));

    input(x0, x1) = (in1 ~+(x0, fb1_fx)), (in2 ~+(x1, fb2_fx));
};

process = scheme;
