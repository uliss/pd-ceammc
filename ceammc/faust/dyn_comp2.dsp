import("compressor.lib");

ratio = max(1, hslider("ratio", 1, 1, 10, 0.001));
thresh = hslider("threshold", 100, 0, 100, 0.1) - 100;
atk = vslider("attack", 10, 1, 100, 0.1) / 1000.0;
rel = vslider("release", 50, 1, 500, 0.1) / 1000.0;

process =  _,_ : compressor_stereo(ratio,thresh,atk,rel) : _,_;
