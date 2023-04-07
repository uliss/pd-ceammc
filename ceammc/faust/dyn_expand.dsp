declare name "dyn.expand";

import("stdfaust.lib");
cm = library("ceammc.lib");
ui = library("ceammc_ui.lib");

process = _ : cm.bypass_mono(0, expand) : _ with {
    N = 1;
    pre = 1;
    max_hold_samp = 192000;
    g = hslider("gain", -6, -60, 0, 0.001) : ba.db2linear;
    expand = co.expander_N_chan(strength, thresh, -32, att, hold, rel, knee, pre, link, meter, max_hold_samp, N);
    strength = hslider("strength", 0, -1, 1, 0.01);
    knee     = hslider("knee [unit:db]", 3, 0, 6, 0.01);
    thresh   = hslider("threshold [unit:db]", -6, -60, 0, 0.1);
    att      = hslider("attack [unit:ms]", 10, 0.1, 100, 0.1) * 0.001;
    hold     = hslider("hold [unit:ms]", 200, 0.1, 1000, 0.1) * 0.001;
    rel      = hslider("release [unit:ms]", 50, 1, 500, 0.1) * 0.001;
    link     = hslider("link", 1, 0, 1, 0.01);


    meter = _ <: attach(_, hbargraph("level", -90, 90));
};

