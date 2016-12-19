mi = library("misceffect.lib");
si = library("signal.lib");
import("ceammc.lib");

thresh = vslider("threshold", 40, 0, 100, 0.1) : db_pd2faust : si.smoo;
atk = vslider("attack", 0.1, 0, 500, 0.1) : time_pd2faust;
hold = vslider("hold", 100, 1, 500, 0.1) : time_pd2faust;
rel = vslider("release", 20, 1, 500, 0.1) : time_pd2faust;

process = mi.gate_stereo(thresh, atk, hold, rel);
