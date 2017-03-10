import("maxmsp.lib");
si = library("signal.lib");

freq = hslider("freq", 1000, 20, 20000, 0.1) : si.smoo;
q = hslider("q", 1, 0.5, 2, 0.1) : si.smoo;
gain = hslider("gain", 0, -15, 15, 0.1) : si.smoo;

process(in) = filtercoeff(freq, gain, q).lowShelf;
