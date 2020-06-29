declare name "flt.bpf24";

fi = library("filters.lib");
ui = library("ceammc_ui.lib");
ma = library("maths.lib");
si = library("signals.lib");

process = fi.bandpass(2, freq_low, freq_high)
with {
    // UI
    freq = ui.freq(1000);
    q = ui.q(2);

    bandwidth = freq / q;
    halfband = bandwidth / 2.0;
    freq_low = max(freq - halfband, 20);
    freq_high = min(freq + halfband, ma.SR * 0.499);
};
