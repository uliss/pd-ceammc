declare name "fx.recho";

cm = library("ceammc.lib");
co = library("compressors.lib");
ef = library("misceffects.lib");
ma = library("maths.lib");
si = library("signals.lib");
ui = library("ceammc_ui.lib");

// phasor in [0..period_samp) range with phase control
phasor(period_samp, phase_samp) = (x + phase_samp) % period_samp : int letrec {
    'x = x + 1;
};

// phasor in [0..1) range with phase control
norm_phasor(period_samp, phase_samp) = phasor(period_samp, phase_samp) : float / period_samp;

// triangle wave: /\/\/\/\
index_tri(period_samp, phase_samp) = x : int with {
    saw0 = phasor(period_samp * 2, phase_samp);
    saw1 = saw0 * -1 + (period_samp * 2);
    sel_second = saw0 > period_samp;
    x = select2(sel_second, saw0, saw1); // fold two saw: positive and negative
};

delay_module(delay_ms, phase_ms) = rwtable(MAX_DELAY, 0.0, write_pos, _, read_pos) : *(hann) with{
    MAX_DELAY = 96000;

    delay_samp = min(MAX_DELAY, delay_ms * 0.001 * ma.SR) : int;
    phase_samp = phase_ms * 0.001 * ma.SR : int;

    write_pos = index_tri(delay_samp, phase_samp);
    read_pos = index_tri(delay_samp, phase_samp+1);
    window = sin(ma.PI * norm_phasor(delay_samp, phase_samp+1));
    hann = window*window;
};

rev_delay(delay_ms) = _ <: d0, d1 :> _ with {
    d0 = delay_module(delay_ms, 0);
    d1 = delay_module(delay_ms, delay_ms/2);
};

fx_recho = fx with {
    min_delay_ms = 0;
    default_delay_ms = 500;
    max_delay_ms = 2000;
    // UI
    delay_time = hslider("delay [unit:ms]", default_delay_ms, min_delay_ms, max_delay_ms, 0.1);
    fb = hslider("feedback", 0.3, 0, 0.99, 0.001) : si.smoo;
    comp = co.compressor_mono(5, -3, 10, 50);

    fx = _ : (+ : rev_delay(delay_time) : comp) ~ *(fb) : _;
};

process = _ : cm.fx_wrap_mono(ui.bypass, ui.drywet(1), fx_recho) : _;
