declare name "flt.bpf36e";

fi = library("filters.lib");
ma = library("maths.lib");
si = library("signals.lib");

process = fi.bandpass6e(freq_low, freq_high)
with {
    freq = vslider("freq [unit:Hz]", 1000, 20, 20000, 0.1);
    q = hslider("q", 2, 0.25, 10, 0.001);

    bandwidth = freq / q;
    halfband = bandwidth / 2.0;
    freq_low = max(freq - halfband, 20);
    freq_high = min(freq + halfband, ma.SR * 0.499);
};
