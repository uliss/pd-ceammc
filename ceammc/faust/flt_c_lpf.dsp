import("maxmsp.lib");
si = library("signal.lib");

freq = hslider("freq", 1000, 20, 20000, 0.1) : si.smoo;
q = hslider("q", 1, 0.01, 15, 1) : si.smoo;

process(in) = filtercoeff(freq, 0, q).LPF;
