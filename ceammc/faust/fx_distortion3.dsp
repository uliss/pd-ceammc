declare name        "fx.distortion3";
declare category    "Distortion";
declare id          "distortion3";
declare description "A simple Wavesharper distortion";

import("stdfaust.lib");
import("guitarix.lib");
cm = library("ceammc.lib");
ui = library("ceammc_ui.lib");

process = _ : cm.fx_wrap_mono(ui.bypass, ui.drywet(1), fx) : _ with {
    fx = bread : *(boost) : dist : butter;

    dist(x)   = (1+k)*(x)/(1+k*abs(x)): +~_''* 0.5 with {
        k     = 2*a/(1-a);
        a     = sin(((drive+1)/102)*(PI/2));
        drive = vslider("drive [tooltip: Distortion Level]", 50, 0, 100, 1): si.smoo;
    };

    bread = highpass(2, hp_freq) : lowpass(1, 6532);
    butter = lowpass(2, lp_freq);

    hp_freq = vslider("hp_freq [unit:Hz]", 30, 20, 2000, 1.08);
    lp_freq = vslider("lp_freq [unit:Hz]", 10000, 1000, 20000, 1.08);
    boost = vslider("gain [unit:db]", 0, -30, 10, 0.1) : db2linear : si.smoo;
};
