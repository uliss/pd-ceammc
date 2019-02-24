declare name "flt.hpf24";

fl = library("filters.lib");
ui = library("ceammc_ui.lib");

process = fl.highpass(4, ui.freq(10000));
