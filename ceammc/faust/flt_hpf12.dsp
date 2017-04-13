fl = library("filters.lib");
ms = library("maxmsp.lib");
ui = library("ceammc_ui.lib");

stage12 = (ms.filtercoeff(ui.freq(10000), 0, ui.q(0.01)).HPF, _) : fl.tf22;

process = stage12;
