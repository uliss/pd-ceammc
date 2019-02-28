declare name "noise.lfreq";

si = library("signals.lib");
no = library("noises.lib");
ma = library("maths.lib");

freq = hslider("freq [unit:Hz]", 1000, 5, 22050, 0.1) : si.smoo;

process = no.lfnoise(freq);
