os = library("oscillators.lib");
si = library("signals.lib");

c = hslider("feedback", 0, 0, 16, 0.1) : si.smoo;

process(freq) = os.oscp(freq) ~ _*c;

