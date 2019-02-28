declare name "dyn.gate2";

ef = library("misceffects.lib");
import("ceammc.lib");

thresh = vslider("threshold [unit:db]", 40, 0, 100, 0.1) : db_pd2faust : si.smoo;
atk    = vslider("attack [unit:ms]", 0.1, 0, 500, 0.1) : time_pd2faust;
hold   = vslider("hold [unit:ms]", 100, 1, 500, 0.1) : time_pd2faust;
rel    = vslider("release [unit:ms]", 20, 1, 500, 0.1) : time_pd2faust;

process = ef.gate_stereo(thresh, atk, hold, rel);
