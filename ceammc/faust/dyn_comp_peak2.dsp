declare name "dyn.comp_peak2";

import("stdfaust.lib");
cm = library("ceammc.lib");
ui = library("ceammc_ui.lib");



process = si.bus(N) : cm.bypass_stereo(ui.bypass, comp) : si.bus(N) with {
    // from compressorsl.ib
    FFcompressor_N_chan(strength,thresh,att,rel,knee,prePost,link,meter,N) =
        si.bus(N) <: (co.peak_compression_gain_N_chan_db(strength,thresh,att,rel,knee,prePost,link,N),si.bus(N)) : ro.interleave(N,2) : par(i,N,(meter(i): ba.db2linear)*_);

    N = 2;
    comp = _,_ : FFcompressor_N_chan(strength, thresh, att, rel, knee, pre, link, meter, N) : *(gain),*(gain);

    pre = 1;
    strength = hslider("strength", 0, 0, 2, 0.01);
    knee     = hslider("knee [unit:db]", 3, 0, 6, 0.01);
    thresh   = hslider("threshold [unit:db]", -10, -60, 0, 0.1);
    att      = hslider("attack [unit:ms]", 10, 0.1, 100, 0.1) * 0.001;
    rel      = hslider("release [unit:ms]", 50, 1, 500, 0.1) * 0.001;
    gain     = vslider("gain [unit:db]", 0, -18, 18, 0.1) : ba.db2linear : si.smoo;
    link     = hslider("link", 1, 0, 1, 0.01);

    meter(i) = _ <: _,_ : attach(_, meter_fx) with {
        meter_fx = ba.db2linear : (1-_) : hbargraph("level%i", 0, 1);
    };
};

