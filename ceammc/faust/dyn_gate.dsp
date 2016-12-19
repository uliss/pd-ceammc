import("misceffect.lib");
import("ceammc.lib");

thresh = hslider("threshold", 40, 0, 100, 0.1) : db_pd2faust;
atk = vslider("attack", 0.1, 0, 500, 0.1) : time_pd2faust;
hold = vslider("hold", 100, 1, 500, 0.1) : time_pd2faust;
rel = vslider("release", 20, 1, 500, 0.1) : time_pd2faust;

process = gate_mono(thresh, atk, hold, rel);
