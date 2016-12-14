import("filter.lib");

freq = hslider("freq", 1000, 20, 20000, 0.1);
gain = vslider("gain", 0, -20, 20, 0.1);
bw = hslider("bandwidth", 100, 1, 5000, 0.1);

process = peak_eq(gain, freq, bw);
