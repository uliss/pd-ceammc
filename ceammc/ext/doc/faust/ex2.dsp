declare name "example2";
import("stdfaust.lib");
amp = hslider("amp", 0.35, 0, 1, 0.001) * 1000: si.smoo;
freq = hslider("freq", 6, 0, 10, 0.001) * 50 : si.smoo;
process = os.osc(200 + (os.osc(freq) * amp)), _ : _, !;


