declare name "fx.ringmod";
declare version "0.1";

cm = library("ceammc.lib");
ui = library("ceammc_ui.lib");
import("stdfaust.lib");

process(car, mod) = cm.fx_wrap_mono(ui.bypass, ui.drywet(0.5), fx_ring(car, mod)) : _ with {
    offset = hslider("offset", 0, 0, 1, 0.001) : si.smoo;
    fx_ring(car, mod) = car * (mod + offset);
};
