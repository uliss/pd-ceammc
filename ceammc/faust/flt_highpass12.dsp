fl = library("filter.lib");
ms = library("maxmsp.lib");
si = library("signal.lib");

freq = hslider("freq", 10000, 20, 20000, 0.1) : si.smoo;
q = vslider("res", 0, -15, 15, 0.1) : si.smoo;

stage12 = (ms.filtercoeff(freq,0,q).HPF, _) : fl.tf21;

process = stage12;
