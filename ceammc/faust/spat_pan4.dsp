sp = library("spats.lib");
ma = library("maths.lib");

TWO_PI = 2.0 * ma.PI;

angle = vslider("angle", 0, 0, TWO_PI, 0.0001);
dist = vslider("dist", 1, 0, 1, 0.0001);

process =  _: sp.spat(4, angle / TWO_PI, dist) : _,_,_,_;
