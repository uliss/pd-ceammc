fl = library("filters.lib");
ui = library("ceammc_ui.lib");

process = fl.lowpass(4, ui.freq(1000));
