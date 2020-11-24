declare name "synth.russian_bell";

spn = library("spn.lib");
pm = library("physmodels.lib");
ba = library("basics.lib");


process = strike : bell
with{
    nmodes = 50;
    strike = pm.strikeModel(10, cutoff, sharpness, gain, gate);
    bell = pm.russianBellModel(nmodes, pos, t60, t60_decay_ratio, t60_decay_slope);

    pos = nentry("pos", 0, 0, 6, 1);
    cutoff = hslider("cutoff [unit:Hz]", 7000, 20, 20000, 1);
    sharpness = hslider("sharp", 0.25, 0.01, 5, 0.01);
    gain = hslider("gain", 1, 0, 1, 0.01);
    gate = button("gate");
    t60 = hslider("t60 [unit:sec]", 5, 0.1, 30, 0.01);
    t60_decay_ratio = hslider("decay_ratio", 1, 0, 1, 0.01);
    t60_decay_slope = hslider("decay_slope", 3, 0, 10, 0.01);
};
