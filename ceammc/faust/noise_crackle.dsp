declare name "noise.crackle";

no = library("noises.lib");

freq = hslider("freq [unit:Hz] [style:knob]", 10, 0, 100, 0.01);
process = no.sparse_noise(freq);
