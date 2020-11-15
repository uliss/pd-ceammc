declare name "spat.zita8";

//sp = library("spats.lib");
ba = library("basics.lib");
si = library("signals.lib");
ma = library("maths.lib");
re = library("reverbs.lib");
cm = library("ceammc.lib");

process = si.bus(6) : par(i, nin,
    zita_gain(
        vslider("delay%i [unit:ms] [style:knob]", 60, 0, 100, 1),
        vslider("decay%i [unit:sec] [style:knob] [scale:log]", 2, 0, 90, 0.1),
        vslider("direct%i [unit:db] [style:knob] [scale:log]", -3, -60, 0, 0.1) : ba.db2linear,
        vslider("reverb%i [unit:db] [style:knob] [scale:log]", -6, -60, 0, 0.1) : ba.db2linear)
    :
    spat(8,
        vslider("angle%i", 0, 0, 1, 0.0001),
        vslider("radius%i", 1, 0, 1, 0.0001))) :> si.bus(8)
with {
    nin = 6;
    nout = 8;
    fsmax = 48000;  // highest sampling rate that will be used

    q8_sqrt = ffunction(float q8_sqrt (float), "m_pd.h", "");

    mix(fx, dry, wet) = _ <: fx *wet, _ *dry :> _;

    zita(ui_delay, ui_decay) = _,0 : re.zita_rev1_stereo(ui_delay, 200, 6000, ui_decay, ui_decay, fsmax) : _, !;

    zita_gain(ui_delay, ui_decay, ui_direct_gain, ui_reverb_gain) = mix(zita(ui_delay, ui_decay), ui_direct_gain, ui_reverb_gain);

    spat(n,a,d) = _ <: par(i, n, *( scaler(i, n, a, d) : si.smooth(0.999) ))
    with {
        scaler(i,n,a,d) = (d/2.0+0.5)
            * q8_sqrt( max(0.0, 1.0 - abs(fmod(a+0.5+float(n-i)/n, 1.0) - 0.5) * n * d) );
    };
};
