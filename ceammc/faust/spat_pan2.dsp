import("spat.lib");

angle = vslider("angle", 0, -1, 1, 0.001) : _ + 1 : _ * 0.5 : si.smoo;
dist = vslider("dist", 1, 0, 1, 0.001) : si.smoo;

process(in) =  in : spat(2, angle, dist);
