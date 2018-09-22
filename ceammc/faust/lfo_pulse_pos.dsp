import("stdfaust.lib");

duty = hslider("duty", 0.5, 0, 1, 0.01);
process(freq) = os.lf_pulsetrainpos(freq, duty);
