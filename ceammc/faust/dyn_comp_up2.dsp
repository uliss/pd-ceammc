declare name "dyn.comp_up2";

import("stdfaust.lib");
cm = library("ceammc.lib");
ui = library("ceammc_ui.lib");

process = _,_ : cm.bypass_stereo(ui.bypass, comp_up) : _,_ with {
    N = 2;
    pre = 1;
    max_hold_samp = 255;

    comp_up  = co.expander_N_chan(strength, thresh, range, att, hold, rel, knee, pre, link, meter, max_hold_samp, N);
    strength = hslider("strength", 0, 0, 1, 0.01) * -1 : si.smoo;
    range    = hslider("range [unit:db]", 12, 0, 32, 0.01) * -1 : si.smoo;
    knee     = hslider("knee [unit:db]", 3, 0, 24, 0.01);
    thresh   = hslider("threshold [unit:db]", -32, -60, 0, 0.1);
    att      = hslider("attack [unit:ms]", 10, 0.1, 100, 0.1) : cm.time_pd2faust;
    hold     = hslider("hold [unit:samp]", 128, 1, max_hold_samp, 0.1) : ba.samp2sec;
    rel      = hslider("release [unit:ms]", 50, 1, 500, 0.1) : cm.time_pd2faust;
    link     = hslider("link", 1, 0, 1, 0.01);

    meter = _ <: attach(_, hbargraph("level [unit:db]", 0, 90)) : _;
};

