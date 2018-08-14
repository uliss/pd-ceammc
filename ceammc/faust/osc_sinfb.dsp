os = library("oscillators.lib");
si = library("signals.lib");
ma = library("maths.lib");

c = hslider("feedback", 0, 0, ma.PI * 2, 0.1) : si.smoo;

process(freq) = os.oscp(freq) ~ _*c;
