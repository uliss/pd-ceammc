declare name "lfo.pulse_pos";

osc = library("ceammc_osc.lib");

process(freq) = sqr with {
    pause = checkbox("pause") > 0.5;
    duty = hslider("duty", 0.5, 0, 1, 0.01);
    init_phase = hslider("phase", 0, 0, 1, 0.001);

    sqr = osc.lfo_pulse_pos(freq, duty, init_phase, pause);
};
