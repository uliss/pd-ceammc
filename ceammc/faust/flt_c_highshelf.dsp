import("maxmsp.lib");

freq = hslider("freq", 10000, 20, 20000, 0.1);
gain = hslider("gain", 0, -15, 15, 0.1);
q = hslider("q", 1, 0.5, 2, 0.1);

process = filtercoeff(freq, gain, q).highShelf;
