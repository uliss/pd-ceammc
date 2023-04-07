declare name "dyn.expand";

import("stdfaust.lib");
cm = library("ceammc.lib");
ui = library("ceammc_ui.lib");

process = _ : cm.bypass_mono(ui.bypass, expand) : _ with {
    N = 1;
    pre = 1;
    max_hold_samp = 255;

    expand  = co.expander_N_chan(strength, thresh, range, att, hold, rel, knee, pre, 0, meter, max_hold_samp, N);
    strength = hslider("strength", 0, 0, 1, 0.01) : si.smoo;
    range    = hslider("range [unit:db]", -120, -120, 0, 0.01);
    knee     = hslider("knee [unit:db]", 3, 0, 6, 0.01);
    thresh   = hslider("threshold [unit:db]", -48, -60, 0, 0.1);
    att      = hslider("attack [unit:ms]", 10, 0.1, 100, 0.1) : cm.time_pd2faust;
    hold     = hslider("hold [unit:samp]", 128, 1, max_hold_samp, 1) : ba.samp2sec;
    rel      = hslider("release [unit:ms]", 50, 1, 500, 0.1) : cm.time_pd2faust;

    meter = _ <: attach(_, hbargraph("level [unit:db]", 0, 90));
};

