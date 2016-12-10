os = library("miscoscillator.lib");
c = hslider("feedback", 0, 0, 4, 0.1);
process(freq) = os.oscp(freq) ~ _*c;

