declare name "dyn.gate2";

import("stdfaust.lib");
cm = library("ceammc.lib");
ui = library("ceammc_ui.lib");

process = cm.bypass_stereo(ui.bypass, gate) with
{
    gate = ef.gate_stereo(thresh, atk, hold, rel);

    thresh = vslider("threshold [unit: db]", -60, -90, 0, 0.1) : si.smoo;
    atk    = vslider("attack [unit:ms]", 0.1, 0, 500, 0.1) : cm.time_pd2faust;
    hold   = vslider("hold [unit:ms]", 100, 1, 500, 0.1) : cm.time_pd2faust;
    rel    = vslider("release [unit:ms]", 20, 1, 500, 0.1) : cm.time_pd2faust;
};
