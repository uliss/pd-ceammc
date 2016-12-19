import("maxmsp.lib");
si = library("signal.lib");

freq = hslider("freq", 10000, 20, 20000, 0.1) : si.smoo;
gain = vslider("gain", 0, -15, 15, 0.1) : si.smoo;

process(x) = highShelf(x, freq, gain, 1);
