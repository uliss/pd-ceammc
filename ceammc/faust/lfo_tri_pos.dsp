declare name "lfo.tri_pos";

osc = library("ceammc_osc.lib");

process(freq) = tri with {
    pause = checkbox("pause") > 0.5;
    init_phase = hslider("phase", 0, 0, 1, 0.001);

    tri = osc.lfo_tri_pos(freq, init_phase, pause);
};

