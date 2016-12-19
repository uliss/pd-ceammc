os = library("miscoscillator.lib");
si = library("signal.lib");

duty = hslider("duty", 0.5, 0, 1.0, 0.001) : si.smoo;

process(freq) = os.pulsetrain(freq, duty);
