declare name "lfo.square_pos";

osc = library("ceammc_osc.lib");

process(freq) = sqr with {
    pause = checkbox("pause") > 0.5;
    init_phase = hslider("phase", 0, 0, 1, 0.001);

    saw = osc.lfo_saw_pos(freq, init_phase, pause);
    sqr = float(saw <= 0.5);
};
