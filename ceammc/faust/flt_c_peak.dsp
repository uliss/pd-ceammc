msp = library("maxmsp.lib");
si = library("signals.lib");
ui = library("ceammc_ui.lib");

process(in) = msp.filtercoeff(ui.freq(1000), ui.gain_db(0), ui.q(1)).peakingEQ;
