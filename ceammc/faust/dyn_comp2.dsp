declare name "dyn.comp2";

ui = library("ceammc_ui.lib");
cm = library("ceammc.lib");
import("stdfaust.lib");

process = ba.bypass2(ui.bypass, comp2) with {
    compressor_stereo(ratio, thresh, att, rel, x, y) = cgm*x, cgm*y with {
        cgm = co.compression_gain_mono(ratio, thresh, atk, rel, abs(x)+abs(y)) <: attach(_, (1-_) : vbargraph("level", 0, 1));
    };

    comp2 = compressor_stereo(ratio, thresh, atk, rel) : _*gain, _*gain;

    ratio  = hslider("ratio", 1, 1, 16, 0.001);
    thresh = vslider("threshold [unit:db]", -10, -90, 0, 0.1);
    atk    = vslider("attack [unit:ms]", 10, 0.1, 100, 0.1) : cm.time_pd2faust;
    rel    = vslider("release [unit:ms]", 50, 1, 500, 0.1) : cm.time_pd2faust;
    gain   = vslider("gain [unit:db]", 0, -18, 18, 0.1) : ba.db2linear : si.smoo;
};
