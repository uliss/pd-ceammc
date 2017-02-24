fl = library("filter.lib");
ms = library("maxmsp.lib");
si = library("signal.lib");

freq = hslider("freq", 10000, 20, 20000, 0.1) : si.smoo;
q = vslider("q", 0.01, 0.01, 15, 0.1) : si.smoo;

stage12 = (ms.filtercoeff(freq,0,q).LPF, _) : fl.tf21;

process = stage12;
