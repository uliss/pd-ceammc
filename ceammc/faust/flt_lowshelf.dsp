msp = library("maxmsp.lib");
ui = library("ceammc_ui.lib");

process(x) = msp.lowShelf(x, ui.freq(1000), ui.gain_db(0), 1);
