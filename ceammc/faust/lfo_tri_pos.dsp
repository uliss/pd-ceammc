declare name "lfo.tri_pos";

osc = library("ceammc_osc.lib");
ma = library("maths.lib");

process(freq) = tri with {
    pause = checkbox("pause") > 0.5;
    init_phase = hslider("phase", 0, 0, 1, 0.001);

    saw = osc.lfo_saw(freq, init_phase, pause);
    tri = 1 - abs(saw);
};

