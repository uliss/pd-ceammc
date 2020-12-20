declare name "flt.notch";

si = library("signals.lib");
fi = library("filters.lib");
ui = library("ceammc_ui.lib");

width = vslider("width [unit:Hz]", 50, 1, 10000, 0.1) : si.smoo;

process = fi.notchw(width, ui.freq(1000));
