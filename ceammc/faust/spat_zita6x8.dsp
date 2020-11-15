declare name "spat.zita8";

//sp = library("spats.lib");
ba = library("basics.lib");
si = library("signals.lib");
ma = library("maths.lib");
re = library("reverbs.lib");
cm = library("ceammc.lib");

process = si.bus(nin) : par(i, nin,
    zita_gain(
        vslider("t%i [unit:sec] [style:knob] [scale:log]", 2, 0, 90, 0.1),
        vslider("dry%i [unit:db] [style:knob] [scale:log]", -3, -60, 0, 0.1) : ba.db2linear,
        vslider("wet%i [unit:db] [style:knob] [scale:log]", -8, -60, 0, 0.1) : ba.db2linear)
    :
    spat(8,
        vslider("a%i", 0, 0, 1, 0.0001), // angle
        vslider("r%i", 1, 0, 1, 0.0001))) :> si.bus(nout) // radius
with {
    nin = 6;
    nout = 8;
    predelay = 40;
    fsmax = 48000;  // highest sampling rate that will be used

    mix(fx, dry, wet) = _ <: fx *wet, _ *dry :> _;

    zita(time) = _,0 : re.zita_rev1_stereo(predelay, 200, 6000, time, time, fsmax) : _, !;

    zita_gain(time, dry, wet) = mix(zita(time), dry, wet);

    spat(n, angle, dist) = _ <: par(i, n, *( scaler(i, n, angle, dist) : si.smooth(0.999) ))
    with {
        q8_sqrt = ffunction(float q8_sqrt (float), "m_pd.h", "");

        scaler(i, n, angle, dist) = (dist / 2.0 + 0.5)
            * q8_sqrt( max(0.0, 1.0 - abs(fmod(angle + 0.5 + float(n-i)/n, 1.0) - 0.5) * n * dist) );
    };
};
