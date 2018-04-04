no = library("noises.lib");
rate = hslider("freq", 10000, 5, 22050, 0.1);

process = no.lfnoise0(rate);
