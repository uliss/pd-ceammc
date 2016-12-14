import("maxmsp.lib");

freq = hslider("freq", 10000, 20, 20000, 0.1);
gain = vslider("gain", 0, -15, 15, 0.1);

process(x) = highShelf(x, freq, gain, 1);
