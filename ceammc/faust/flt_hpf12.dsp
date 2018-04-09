fl = library("filters.lib");
ui = library("ceammc_ui.lib");

process = fl.highpass(2, ui.freq(10000));
