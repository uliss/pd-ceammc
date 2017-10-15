msp = library("maxmsp.lib");
ui = library("ceammc_ui.lib");

process(x) = msp.highShelf(x, ui.freq(10000), ui.gain_db(0), 1);
