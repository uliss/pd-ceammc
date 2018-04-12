declare name "ks";

ui = library("ceammc_ui.lib");
spn = library("spn.lib");
pm = library("physmodels.lib");
ba = library("basics.lib");

process = pm.ks(len, 1.0 - mute, excitation) with {
    len = pm.f2l(freq);
    freq = hslider("pitch", spn.C3, spn.C2, spn.C6, 0.001) : ba.midikey2hz;
    mute = hslider("mute", 1, 0, 1, 0.001); // mute coefficient (1 for no mute and 0 for instant mute)

    N = 3;
    mode = hslider("mode", 0, 0, N, 1) : int;
    trigger = button("gate");
    excitation = exc_imp, exc_strike, exc_pluck : ba.selectn(N, mode);
    exc_imp = pm.impulseExcitation(trigger);
    exc_strike = pm.strike(pos, sharp, gain, trigger);
    exc_pluck = pm.pluckString(len, cutoff, 1, sharp, gain, trigger);

    pos = hslider("pos", 0.5, 0, 1, 0.01); // excitation position (0-1) (1 is on the bridge)
    sharp = hslider("sharp", 0.25, 0.01, 1, 0.001); // sharpness of the strike (recommened: ~0.25)
    gain = hslider("gain", 1, 0, 1, 0.001); // pluck gain
    cutoff = hslider("cutoff", 1, 0, 1, 0.001);
};
