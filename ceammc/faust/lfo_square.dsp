declare name "lfo.square";

osc = library("ceammc_osc.lib");

process(freq) = sqr with {
    pause = checkbox("pause") > 0.5;
    init_phase = hslider("phase", 0, 0, 1, 0.001);

    sqr = osc.lfo_square(freq, init_phase, pause);
};
