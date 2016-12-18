import("filter.lib");
si = library("signal.lib");

freq = vslider("freq", 1000, 20, 20000, 0.1) : si.smoo;
gain = vslider("gain", 0, -20, 20, 0.1) : si.smoo;
bw = vslider("bandwidth", 100, 1, 5000, 0.1) : si.smoo;

process = peak_eq(gain, freq, bw);
