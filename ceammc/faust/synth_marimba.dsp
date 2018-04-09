declare name "marimba";

ui = library("ceammc_ui.lib");
spn = library("spn.lib");
ba = library("basics.lib");
pm = library("physmodels.lib");

process = pm.marimba(freq, strikePosition, strikeCutoff, strikeSharpness, 1, trigger) with {
    freq = hslider("pitch", spn.C(3), spn.C(2), spn.C(6), 0.001) : ba.midikey2hz;
    strikePosition = hslider("pos", 0, 0, 4, 0.01);
    strikeCutoff = hslider("cutoff", 7000, 1000, 10000, 1); // cuttoff frequency of the strike generator
    strikeSharpness = hslider("sharp", 0.25, 0.01, 1, 0.001); // sharpness of the strike (recommened: ~0.25)
    strikeGain = hslider("gain", 1, 0, 1, 0.001); // strike gain
    trigger = button("gate");
};
