declare name "synth.voice_fofc";

ba = library("basics.lib");
en = library("envelopes.lib");
os = library("oscillators.lib");
pm = library("physmodels.lib");
si = library("signals.lib");
ui = library("ceammc_ui.lib");


formantFilterFofCycle(voiceType,vowel,nFormants,i,freq) =
        pm.fofCycle(formantFreq(i),formantBw(i),formantSw(i),formantGain(i),n)
with{
        n = 12; // number of fof filters to cycle between
        index = (voiceType*nFormants)+vowel; // index of formant values
        // formant center frequency using autobend correction
        formantFreq(i) = ba.listInterp(pm.formantValues.f(i),index) : pm.autobendFreq(i,freq,voiceType);
        // formant amplitude using vocal effort correction
        formantGain(i) = ba.listInterp(pm.formantValues.g(i),index) : pm.vocalEffort(freq,gender);
        formantBw(i) = ba.listInterp(pm.formantValues.bw(i),index); // formant bandwidth
        // formant skirtwidth
        formantSw(i) = pm.skirtWidthMultiplier(vowel,freq,gender)*formantBw(i);
        gender = pm.voiceGender(voiceType); // gender of voice
};


formantFilterFofSmooth(voiceType,vowel,nFormants,i,freq) =
        pm.fofSmooth(formantFreq(i),formantBw(i),formantSw(i),formantGain(i),tau)
with{
        tau = 0.05;
        index = (voiceType*nFormants)+vowel; // index of formant values
        // formant center frequency using autobend correction
        formantFreq(i) = ba.listInterp(pm.formantValues.f(i),index) : pm.autobendFreq(i,freq,voiceType);
        // formant amplitude using vocal effort correction
        formantGain(i) = ba.listInterp(pm.formantValues.g(i),index) : pm.vocalEffort(freq,gender);
        formantBw(i) = ba.listInterp(pm.formantValues.bw(i),index); // formant bandwidth
        // formant skirtwidth
        formantSw(i) = pm.skirtWidthMultiplier(vowel,freq,gender)*formantBw(i);
        gender = pm.voiceGender(voiceType); // gender of voice
};

process = model * envelope with {
    fofBank(voiceType, vowel, freq) = pm.formantFilterbank(voiceType, vowel, formantFilterFofCycle, freq);
    model = pm.SFFormantModel(voice, vowel, 0, freq2, 50, os.lf_imptrain(freq2), fofBank, 1);
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
