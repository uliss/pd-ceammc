declare name "lfo.saw_pos";

osc = library("ceammc_osc.lib");
ma = library("maths.lib");


process(freq) = saw with {
    pause = checkbox("pause") > 0.5;
    init_phase = hslider("phase", 0, 0, 1, 0.001);
    iv = checkbox("invert") > 0.5;

    iv1 = 1 - 2 * iv; // 1 or -1
    zm_saw = osc.lfo_saw_pos(freq, init_phase, pause) * 2 - 1;
    saw = zm_saw * iv1 : + (1) : * (0.5);
};
