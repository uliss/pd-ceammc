declare name "fx.jcrev";

re = library("reverbs.lib");
cm = library("ceammc.lib");
ui = library("ceammc_ui.lib");
import("stdfaust.lib");

process = _ <: chain : _,_,_,_ with {
    mix2(c, x, y) = x*(1-c) + y*c;
    drywet4(c,in) = in,!,!,! : re.jcrev : mix2(c,in), mix2(c,in), mix2(c,in), mix2(c,in);
    bypass4(fx) = _,_,_,_ : ba.bypass_fade(ma.SR/20, ui.bypass, fx) : _,_,_,_;

    fx(in) = drywet4(ui.drywet(0.4), in);
    chain = _ <: bypass4(fx);
};
