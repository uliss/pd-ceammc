import("stdfaust.lib");

a = hslider("attack", 10, 0, 100000, 1) / 1000.0;
d = hslider("decay", 10, 0, 100000, 1) / 1000.0;
s = hslider("sustain", 50, 0, 100, 0.001);
r = hslider("release", 10, 0, 100000, 1) / 1000.0;
t = hslider("trigger", 0, 0, 1, 0.001);
process = _ * en.adsr(a,d,s,r,t);
