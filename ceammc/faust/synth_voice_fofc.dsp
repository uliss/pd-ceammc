declare name "synth.voice_fofc";

ba = library("basics.lib");
en = library("envelopes.lib");
os = library("oscillators.lib");
pm = library("physmodels.lib");
si = library("signals.lib");
ui = library("ceammc_ui.lib");

process = pm.SFFormantModelFofCycle(voice_t, vowel, freq2, 75) * envelope with {
    voice_t = hslider(".voice", 0, 0, 5, 0.001) : int;
    vowel = hslider(".vowel", 0, 0, 4, 0.001) : si.smoo;
    gliss = hslider("portamento", 0, 0, 4000, 0.001) : si.smoo; // in milliseconds
    freq = ui.freq(440) : si.smooth(ba.tau2pole(gliss * 0.001));

    vibrato_freq = hslider("vibr_freq", 6, 1, 10, 0.01);
    vibrato_gain = hslider("vibr_gain", 0.5, 0, 1, 0.01) * 0.1;
    freq2 = freq * (os.osc(vibrato_freq)*vibrato_gain + 1);

    attack = hslider("attack [unit:msec]", 10, 0, 1000, 0.01) * 0.001;
    release = hslider("release [unit:msec]", 10, 0, 1000, 0.01) * 0.001;
    envelope = en.are(attack, release, ui.gate);
};
