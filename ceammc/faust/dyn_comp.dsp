declare name "dyn.comp";

import("ceammc.lib");
co = library("compressors.lib");

ratio  = vslider("ratio", 1, 1, 10, 0.001);
thresh = vslider("threshold [unit:db]", 100, 0, 100, 0.1) : db_pd2faust;
atk    = vslider("attack [unit:ms]", 10, 1, 100, 0.1) : time_pd2faust;
rel    = vslider("release [unit:ms]", 50, 1, 500, 0.1) : time_pd2faust;
gain   = vslider("gain [unit:db]", 0, -12, 12, 0.1) : ba.db2linear : si.smoo;

process = co.compressor_mono(ratio, thresh, atk, rel) * gain;
