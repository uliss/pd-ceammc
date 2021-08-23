declare name "lfo.mosc";

ba = library("basics.lib");
ma = library("maths.lib");
osc = library("ceammc_osc.lib");

process(freq) = wave with {
    pause = checkbox("pause") > 0.5;
    init_phase = hslider("phase", 0, 0, 1, 0.001);
    duty = hslider("duty", 0.5, 0, 1, 0.01);
    windex = nentry("windex [type:int]", 0, 0, 9, 1);

    psin = osc.lfo_sin_pos(freq, init_phase, pause);
    psaw = osc.lfo_saw_pos(freq, init_phase, pause);
    ptri = osc.lfo_tri_pos(freq, init_phase, pause);
    psqr = osc.lfo_square_pos(freq, init_phase, pause);
    ppulse = osc.lfo_pulse_pos(freq, duty, init_phase, pause);
    wsin = osc.lfo_sin(freq, init_phase, pause);
    saw = osc.lfo_saw(freq, init_phase, pause);
    tri = osc.lfo_tri(freq, init_phase, pause);
    sqr = osc.lfo_square(freq, init_phase, pause);
    pulse = osc.lfo_pulse(freq, duty, init_phase, pause);

    wave = ba.selectmulti(ma.SR/20, (wsin,saw,tri,sqr,pulse,psin,psaw,ptri,psqr,ppulse), windex);
};

