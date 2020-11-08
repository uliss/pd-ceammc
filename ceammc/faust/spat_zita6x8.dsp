declare name "spat.zita8";

//sp = library("spats.lib");
si = library("signals.lib");
ma = library("maths.lib");
re = library("reverbs.lib");
cm = library("ceammc.lib");

fromN1 = _,0;
toN1 = _,_ : _, !;

process = si.bus(6) : par(i, nin,
    cm.fx_drywet(_, fromN1 : re.zita_rev1_stereo(
        vslider("delay%i [unit:ms] [style:knob]", 60, 20, 100, 1),
        200, 6000,
        vslider("decay_low%i [unit:sec] [style:knob] [scale:log]", 3, 1, 10, 0.1),
        vslider("decay_mid%i [unit:sec] [style:knob] [scale:log]", 2, 1, 10, 0.1),
        fsmax)
    : toN1,
        hslider("drywet%i [style:knob]", 0.5, 0, 1, 0.01) : si.smoo)
    : spat(8,
        vslider("angle%i", 0, 0, 1, 0.0001),
        vslider("radius%i", 1, 0, 1, 0.0001))) :> si.bus(8)
with {
    nin = 6;
    nout = 8;
    fsmax = 96000;  // highest sampling rate that will be used

    q8_sqrt = ffunction(float q8_sqrt (float), "m_pd.h", "");

    spat(n,a,d) = _ <: par(i, n, *( scaler(i, n, a, d) : si.smooth(0.9999) ))
    with {
        scaler(i,n,a,d) = (d/2.0+0.5)
            * q8_sqrt( max(0.0, 1.0 - abs(fmod(a+0.5+float(n-i)/n, 1.0) - 0.5) * n * d) );
    };
};
