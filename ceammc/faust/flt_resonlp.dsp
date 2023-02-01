declare name "flt.resonlp";

import("stdfaust.lib");

freq = vslider("freq [unit:Hz]", 1000, 20, 20000, 0.1) : si.smoo;
Q = vslider("q", 40, 0.1, 100, 0.1) : si.smoo;

process = fi.resonlp(freq, Q, 1);
