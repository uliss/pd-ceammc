an = library("analyzers.lib");
cm = library("ceammc.lib");

period = hslider("period [unit:ms]", 200, 1, 1000, 0.1) : cm.time_pd2faust;

process = an.rms_envelope_rect(period);
