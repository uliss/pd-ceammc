import("compressor.lib");
import("ceammc.lib");

ratio = vslider("ratio", 1, 1, 10, 0.001);
thresh = vslider("threshold", 100, 0, 100, 0.1) : db_pd2faust;
atk = vslider("attack", 10, 1, 100, 0.1) : time_pd2faust;
rel = vslider("release", 50, 1, 500, 0.1) : time_pd2faust;

process =  compressor_stereo(ratio,thresh,atk,rel);
