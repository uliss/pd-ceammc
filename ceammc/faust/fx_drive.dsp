declare name "fx.drive";

import("guitarix.lib");
si = library("signals.lib");
ba = library("basics.lib");
ui = library("ceammc_ui.lib");

process = ba.bypass1(ui.bypass, fx) with {
    fx = fuzzy_tube(a, b, c, fuzzy);
    a = 4;
    b = 4;
    c = 0.125;
    fuzzy = vslider("drive", 1, 1, 10, 1) : si.smoo;
};
