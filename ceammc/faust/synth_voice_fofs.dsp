declare name "synth.voice_fofs";

ba = library("basics.lib");
en = library("envelopes.lib");
os = library("oscillators.lib");
pm = library("physmodels.lib");
si = library("signals.lib");
ui = library("ceammc_ui.lib");


process = pm.SFFormantModelFofSmooth(voice, vowel, freq2, 50) * envelope with {
    voice = hslider("ivoice", 0, 0, 5, 0.001) : int;
    vowel_smooth = hslider("wsmooth [unit:ms]", 10, 0, 1000, 0.001); // in milliseconds
    vowel = hslider("fvowel", 0, 0, 4, 0.001) : si.smooth(ba.tau2pole(vowel_smooth * 0.001));
    gliss = hslider("portamento [unit:ms]", 0, 0, 4000, 0.001); // in milliseconds
    freq = ui.freq(440) : si.smooth(ba.tau2pole(gliss * 0.001));

    vibrato_freq = hslider("vibr_freq [unit:Hz]", 6, 1, 10, 0.01);
    vibrato_gain = hslider("vibr_gain", 0.5, 0, 1, 0.01) * 0.1;
    freq2 = freq * (os.osc(vibrato_freq)*vibrato_gain + 1);

    attack = hslider("attack [unit:ms]", 30, 0, 1000, 0.01) * 0.001;
    release = hslider("release [unit:ms]", 50, 0, 1000, 0.01) * 0.001;
    envelope = en.asr(attack, 1, release, ui.gate);
};
