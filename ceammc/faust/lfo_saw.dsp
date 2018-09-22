import("stdfaust.lib");

process = os.lf_sawpos_phase;

//process = zero_mean_saw with {
//    iv = hslider("invert", 0, 0, 1, 0.001) > 0.5;
//    phase = hslider("phase", 0, 0, 1, 0.001);
//    zero_mean_saw(freq) = os.lf_sawpos_phase(freq, phase) * 2 - 1;
//};
