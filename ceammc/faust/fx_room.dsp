declare id "fx.room";
declare author "thedrgreenthumb";
declare license "MIT";

// https://github.com/thedrgreenthumb/faust

import("stdfaust.lib");
ui = library("ceammc_ui.lib");

//Controls
p_predelay = hslider("predelay [unit:ms]", 20, 1, 200, 1);
p_rsize = hslider("size", 1, 0, 3, 0.05);
p_rt = hslider("fb", 0.3, 0, 1, 0.05);
p_hfdamp = 1 - hslider("hfdamp", 1, 0, 1, 0.05);

ms2sps(t) = ba.sec2samp(t * 0.001);
mix2(c,x,y) = (1-c)*x + c*y;
mix3(val) = _,_,_<:select3(val),select3(val+1):mix2(val-floor(val));

// Allpasses
allpass(dt, fb, f_nest) = (+ <:
    (de.delay(maxdel, dt-1) : f_nest),*(fb)) ~ *(-fb) : mem,_ : +
with {
    maxdel = 8192;
};

sn_allpass(dt1,dt2,fb1,fb2) = allpass(dt1,fb1,allpass(dt2,fb2,_));
dn_allpass(dt1,dt2,dt3,fb1,fb2,fb3) =
        allpass(dt1,fb1,allpass(dt2,fb2,allpass(dt3,fb3,_)));

// Gardners room emulation algorithms
small_room(rt, hf_damping) = fi.lowpass(4, 6000):
        ((_,_:>@(ms2sps(24)):dn_allpass(ms2sps(35), ms2sps(22),
        ms2sps(8.3),0.15,0.25,0.3)<:
        sn_allpass(ms2sps(66),ms2sps(30),0.08,0.3),_)~
        (_*0.99:fi.highpass(2,800):fi.lowpass(2,1600) : _*rt : fi.pole(hf_damping))):
        _*0.5,_*0.5:>_;

medium_room(rt, hf_damping,x) = x:fi.lowpass(4,6000):
        ((_,_:>dn_allpass(ms2sps(35),ms2sps(8.3),ms2sps(22),0.25,0.35,0.45)<:
        @(ms2sps(5)),_*0.5:allpass(ms2sps(30),0.45,_),_:(@(ms2sps(67))<:_,_*0.5),_:
        (@(ms2sps(15)):_*rt),_+_:
        (_*0.4:_+x:sn_allpass(ms2sps(39),ms2sps(9.8),0.25,0.35)),_)~
        (fi.highpass(2,500):fi.lowpass(2,1000):fi.pole(hf_damping):_*rt:_*1.299)):
        _*0.5,_:_+_;

large_room(rt, hf_damping) = fi.lowpass(4,4000):
        ((_,_:>allpass(ms2sps(8),0.3,_):allpass(ms2sps(12),0.3,_):
        @(ms2sps(4))<:_,_*1.5:((@(ms2sps(17)):
        sn_allpass(ms2sps(87),ms2sps(62),0.5,0.25):@(ms2sps(31))<:_,_*0.8),_:_,_+_):
        ((@(ms2sps(3)):
        dn_allpass(ms2sps(120),ms2sps(76),ms2sps(30),0.5,0.25,0.25)),_))~
        (_*0.5:_*rt:fi.pole(hf_damping):fi.highpass(2,500):fi.lowpass(2,1000):_*1.299)):_+_*0.8;


process = ba.bypass1(ui.bypass, fx)
with {
    pre_delay = de.fdelay1s(ms2sps(p_predelay));
    small = small_room(p_rt, p_hfdamp);
    medium = medium_room(p_rt, p_hfdamp);
    large = large_room(p_rt, p_hfdamp);
    room = pre_delay <: small, medium, large : mix3(p_rsize);
    fx = _ <: room, _ : mix2(1 - ui.drywet(0.5));
};
