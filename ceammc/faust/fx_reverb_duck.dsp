declare id "fx.reverb_duck";

//------------------------------------
//Inspired by:
//http://www.gvst.co.uk/gduckdly.htm
//Axe-FX II Owner's manual:5.6
//------------------------------------

an = library("analyzers.lib");
ba = library("basics.lib");
cm = library("ceammc.lib");
ef = library("effects.lib");
mu = library("musics.lib");
re = library("reverbs.lib");
si = library("signals.lib");
ui = library("ceammc_ui.lib");

process = _<:_,(_<:(_:rev),
        (ampfollow:_*p_amount:_>1:(1 - _):smoo)):_,_*_
        :>_
with {
    attack_time  = vslider("attack [unit:ms]", 100, 20, 500, 0.1) : cm.time_pd2faust;
    release_time = vslider("release [unit:ms]", 100, 20, 2000, 0.1) : cm.time_pd2faust;
    p_amount     = hslider("level[unit:db]", 0.5, 0, 56, 0.05) : ba.db2linear;

    ampfollow = an.amp_follower_ud(attack_time, release_time);

    c_channels_sw_time = 0.1;
    smoo = si.smooth(ba.tau2pole(c_channels_sw_time));

    rev = _ <: re.zita_rev1_stereo(20, f1, f2, t60, t60, fsmax) : _,! with {
        fsmax = 96000;  // highest sampling rate that will be used
        t60 = vslider("decay [unit:sec] [style:knob] [scale:log]", 4, 1, 6, 0.1);

        rdel = 20;
        f1 = vslider("freq_low [unit:Hz] [style:knob] [scale:log]", 200, 50, 1000, 1);
        f2 = vslider("dump_hf [unit:Hz] [style:knob] [scale:log]", 6000, 1500, 0.49 * fsmax, 1);
    };
};
