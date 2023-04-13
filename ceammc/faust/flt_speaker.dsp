declare name "flt.speaker";

import("stdfaust.lib");
cm = library("ceammc.lib");
ui = library("ceammc_ui.lib");

process = cm.fx_wrap_mono(ui.bypass, ui.drywet(1), filter : *(gain)) with {
    filter = ef.speakerbp(f1, f2);
    f1 = hslider("f1 [unit:hz]", 130, 20, 1000, 1) : si.smoo;
    f2 = hslider("f2 [unit:hz]", 5000, 1000, 20000, 1) : si.smoo;
    gain = hslider("gain [unit:db]", 0, -30, 30, 0.1) : ba.db2linear : si.smoo;
};
