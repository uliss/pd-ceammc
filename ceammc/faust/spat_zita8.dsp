declare name "spat.zita8";

//sp = library("spats.lib");
si = library("signals.lib");
ma = library("maths.lib");
re = library("reverbs.lib");

q8_sqrt = ffunction(float q8_sqrt (float), "m_pd.h", "");

spat(n,a,d) = _ <: par(i, n, *( scaler(i, n, a, d) : si.smooth(0.9999) ))
with {
    scaler(i,n,a,d) = (d/2.0+0.5)
        * q8_sqrt( max(0.0, 1.0 - abs(fmod(a+0.5+float(n-i)/n, 1.0) - 0.5) * n * d) );
};

process =  _ : reverb : spat(8, angle, dist)
with {
    fsmax = 96000;  // highest sampling rate that will be used

    angle = vslider("angle", 0, 0, 1, 0.0001);
    dist = vslider("radius", 1, 0, 1, 0.0001);
    rdel = vslider("delay [unit:ms] [style:knob]", 60, 20, 100, 1) : si.smoo;
    f1 = vslider("freq_low [unit:Hz] [style:knob] [scale:log]", 200, 50, 1000, 1);
    f2 = vslider("dump_hf [unit:Hz] [style:knob] [scale:log]", 6000, 1500, 0.49 * fsmax, 1);
    t60dc = vslider("decay_low [unit:sec] [style:knob] [scale:log]", 3, 1, 60, 0.1);
    t60m = vslider("decay_mid [unit:sec] [style:knob] [scale:log]", 2, 1, 60, 0.1);

    reverb = _, 0 : re.zita_rev1_stereo(rdel, f1, f2, t60dc, t60m, fsmax) : _, !;
};
