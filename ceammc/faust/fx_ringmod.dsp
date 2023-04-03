declare name "fx.ringmod";
declare version "0.1";

cm = library("ceammc.lib");
ui = library("ceammc_ui.lib");
import("stdfaust.lib");

process(car, mod) = fx_bypass(car, mod_src(mod), ui.bypass) : fi.dcblocker : _ with
{
    fx_saturate = _ * hslider("drive", 1, 1, 10, 0.001) : ma.tanh;

    offset = hslider("offset", 0, 0, 1, 0.001) : si.smoo;
    fx_ring(car, mod) = car * (mod + offset) : fx_saturate;

    mix = ui.drywet(0.5);
    fx_drywet(car, mod) = ((mix)*fx_ring(car, mod)), ((1-mix)*car) :> _;
    fx_bypass(car, mod, x) = ((1-x)*fx_drywet(car, mod)), (x*car) :> _;

    mod_src(mod) = (mod, osc) : ba.selectn(2, checkbox("internal")) : _ with {
        osc = os.oscsin(ui.freq(1000));
    };
};
