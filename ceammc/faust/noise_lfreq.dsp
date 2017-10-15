fl = library("filter.lib");
ms = library("maxmsp.lib");
si = library("signal.lib");
no = library("noise.lib");

freq = hslider("freq", 10000, 20, 20000, 0.1) : si.smoo;

process = no.lfnoise(freq);
