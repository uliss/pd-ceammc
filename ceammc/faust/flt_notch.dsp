import("stdfaust.lib");

freq = vslider("freq [unit:Hz]", 10000, 20, 20000, 0.1) : si.smoo;
width = vslider("width [unit:Hz]", 50, 1, 10000, 0.1) : si.smoo;

process = fi.notchw(width, freq);
