declare name "noise.lfreq0";

no = library("noises.lib");
rate = hslider("freq [unit:Hz]", 1000, 0.5, 22050, 0.1);

process = no.lfnoise0(rate);
