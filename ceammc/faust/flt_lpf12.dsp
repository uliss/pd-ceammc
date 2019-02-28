declare name "flt.lpf12";

fl = library("filters.lib");
ui = library("ceammc_ui.lib");

process = fl.lowpass(2, ui.freq(1000));
