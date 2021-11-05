declare name "lfo.saw_pos";

osc = library("ceammc_osc.lib");
ma = library("maths.lib");

process(freq) = saw with {
    pause = checkbox("pause") > 0.5;
    init_phase = hslider("phase", 0, 0, 1, 0.001);
    iv = checkbox("invert") > 0.5;

    saw = osc.lfo_saw_pos(freq, init_phase, pause) : osc.invert_pos(iv);
};
