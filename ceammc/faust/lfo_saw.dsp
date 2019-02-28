import("stdfaust.lib");

process = zero_mean_saw with {
    iv = checkbox("invert") > 0.5;
    iv1 = 1 - 2 * iv;
    zero_mean_saw = os.lf_sawpos * 2 - 1 : * (iv1);
};
