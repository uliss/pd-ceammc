msp = library("maxmsp.lib");
si = library("signals.lib");
ui = library("ceammc_ui.lib");

q = hslider("q", 1, 0.5, 2, 0.1) : si.smoo;

process(in) = msp.filtercoeff(ui.freq(10000), ui.gain_db(0), q).highShelf;
