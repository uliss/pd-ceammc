declare name "flt.resonhp";

import("stdfaust.lib");

freq = vslider("freq [unit:Hz]", 100, 20, 20000, 0.1) : si.smoo;
Q = vslider("q", 20, 0.1, 100, 0.1) : si.smoo;

process = fi.resonhp(freq, Q, 1);
