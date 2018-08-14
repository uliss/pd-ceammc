fi = library("filters.lib");
si = library("signals.lib");
ui = library("ceammc_ui.lib");

Q = vslider("q", 3, 0.1, 100, 0.1) : si.smoo;

process = fi.peak_eq_cq(ui.gain_db(0), ui.freq(1000), Q);
