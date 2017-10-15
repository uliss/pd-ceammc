os = library("oscillators.lib");
si = library("signals.lib");
ba = library("basics.lib");

duty = hslider("duty", 0.5, 0, 1.0, 0.001) : si.smoo;

process(freq) = ba.if(freq == 0, 0, os.pulsetrain(freq, duty));
