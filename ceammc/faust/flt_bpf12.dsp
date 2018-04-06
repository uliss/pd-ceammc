fl = library("filters.lib");
ui = library("ceammc_ui.lib");
ma = library("maths.lib");

process = fl.bandpass(1, freq_low, freq_high)
with {
    // UI
    freq = ui.freq(1000);
    q = ui.q(2);

    bandwidth = freq / q;
    halfband = bandwidth / 2.0;
    freq_low = max(freq - halfband, 0);
    freq_high = min(freq + halfband, ma.SR * 0.5);
};
