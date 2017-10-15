fi = library("filters.lib");
si = library("signals.lib");
ui = library("ceammc_ui.lib");

bw = vslider("bandwidth", 100, 1, 5000, 0.1) : si.smoo;

process = fi.peak_eq(ui.gain_db(0), ui.freq(1000), bw);
