import("stdfaust.lib");
amp = hslider("amp", 0.25, 0, 0.5, 0.001) : si.smoo;
freq = hslider("freq", 2, 0, 10, 0.001) : si.smoo;
process = _ * (amp * os.lf_trianglepos(freq) + 0.5);
