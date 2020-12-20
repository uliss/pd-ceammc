declare name "synth.marimba";

ui = library("ceammc_ui.lib");
cm = library("ceammc.lib");
spn = library("spn.lib");
ba = library("basics.lib");
pm = library("physmodels.lib");
si = library("signals.lib");

process = pm.marimba(freq, strikePosition, strikeCutoff, strikeSharpness, strikeGain, trigger) : _ with {
    freq = hslider("pitch", spn.C3, spn.C2, spn.C6, 0.001) : ba.midikey2hz;
    strikePosition = hslider("pos", 0, 0, 4, 0.01);
    strikeCutoff = hslider("cutoff", 7000, 1000, 10000, 1); // cuttoff frequency of the strike generator
    strikeSharpness = hslider("sharp", 0.25, 0.01, 1, 0.001); // sharpness of the strike (recommened: ~0.25)
    strikeGain = hslider("gain", 1, 0, 1, 0.001); // strike gain
    trigger = button("gate");
    amp = trigger : cm.clip(0, 1) : si.lag_ud(0, 1000);
};
