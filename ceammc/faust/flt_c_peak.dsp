import("maxmsp.lib");
si = library("signal.lib");

freq = hslider("freq", 1000, 20, 20000, 0.1) : si.smoo;
q = hslider("q", 1, 0.1, 100, 0.1) : si.smoo;
gain = vslider("gain", 0, -90, 10, 0.1) : si.smoo;

process(in) = filtercoeff(freq, gain, q).peakingEQ;
