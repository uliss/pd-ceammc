declare name "dyn.comp";

ui = library("ceammc_ui.lib");
cm = library("ceammc.lib");
import("stdfaust.lib");

process = cm.bypass_mono(ui.bypass, comp * gain) with {
    compressor_mono(ratio, thresh, atk, rel, x) = x * knee with {
        knee = co.compression_gain_mono(ratio, thresh, atk, rel, x) <: attach(_, (1-_) : vbargraph("level", 0, 1));
    };

    comp = compressor_mono(ratio, thresh, atk, rel);

    ratio  = hslider("ratio", 1, 1, 16, 0.001);
    thresh = vslider("threshold [unit:db]", -10, -90, 0, 0.1);
    atk    = vslider("attack [unit:ms]", 10, 0.1, 100, 0.1) : cm.time_pd2faust;
    rel    = vslider("release [unit:ms]", 50, 1, 500, 0.1) : cm.time_pd2faust;
    gain   = vslider("gain [unit:db]", 0, -18, 18, 0.1) : ba.db2linear : si.smoo;
};
