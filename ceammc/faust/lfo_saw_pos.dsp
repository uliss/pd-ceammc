import("stdfaust.lib");

process = saw with {
    iv = hslider("invert", 0, 0, 1, 0.001) > 0.5;
    iv1 = 1 - 2 * iv;
    zm_saw = os.lf_sawpos * 2 - 1;
    saw = zm_saw * iv1 : + (1) : * (0.5);
};
