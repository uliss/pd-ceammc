declare name "synth.ks";

ui = library("ceammc_ui.lib");
cm = library("ceammc.lib");
spn = library("spn.lib");
pm = library("physmodels.lib");
ba = library("basics.lib");

process = pm.ks(len, mute, excitation) : _ with {
    len = pm.f2l(freq);
    freq = hslider("pitch", spn.C3, spn.C2, spn.C6, 0.001) : ba.midikey2hz;
    mute = hslider("mute", 0, 0, 1, 0.001); // mute coefficient (0 for no mute and 1 for instant mute)

    N = 2;
    mode = hslider("mode [enum:0 1] [type:int]", 0, 0, N - 1, 1) : int;
    trigger = button("gate");
    amp = trigger : cm.clip(0, 1);
    excitation = exc_pluck, exc_strike : ba.selectn(N, mode);
    exc_imp = pm.impulseExcitation(trigger);
    exc_strike = pm.strike(pos, sharp, gain, trigger);
    exc_pluck = pm.pluckString(len, cutoff, 1, sharp, gain, trigger);

    pos = hslider("pos", 0.7, 0, 1, 0.01); // excitation position (0-1) (1 is on the bridge)
    sharp = hslider("sharp", 0.25, 0.01, 1, 0.001); // sharpness of the strike (recommened: ~0.25)
    gain = hslider("gain", 1, 0, 1, 0.001); // pluck gain
    cutoff = hslider("cutoff", 1, 0.1, 1, 0.001);
};
