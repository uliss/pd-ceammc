declare name "spat.pan8";

//sp = library("spats.lib");
si = library("signals.lib");
ma = library("maths.lib");

q8_sqrt = ffunction(float q8_sqrt (float), "m_pd.h", "");

TWO_PI = 2.0 * ma.PI;

angle = vslider("angle [unit:rad]", 0, 0, TWO_PI, 0.0001);
dist = vslider("radius", 1, 0, 1, 0.0001);

spat(n,a,d)	= _ <: par(i, n, *( scaler(i, n, a, d) : si.smooth(0.9999) ))
with {
        scaler(i,n,a,d) = (d/2.0+0.5)
                * q8_sqrt( max(0.0, 1.0 - abs(fmod(a+0.5+float(n-i)/n, 1.0) - 0.5) * n * d) );
};

process =  _: spat(8, angle / TWO_PI, dist);
