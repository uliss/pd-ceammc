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

    mod_src(mod) = (mod, osc, noise) : ba.selectn(3, nentry("src [type:int] [style:menu{'mod':0,'osc':1,'noise':2}]", 0, 0, 2, 1)) : _ with {
        freq = hslider("freq [unit:Hz]", 150, 0.5, 22050, 0.1);

        osc = os.oscsin(freq);
        noise = no.lfnoise0(freq);
    };
};
