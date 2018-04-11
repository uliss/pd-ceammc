declare name "eguitar";

ui = library("ceammc_ui.lib");
spn = library("spn.lib");
pm = library("physmodels.lib");
ba = library("basics.lib");

process = pm.elecGuitar(len, pluckPosition, mute, pluckGain, trigger) with {
    len = pm.f2l(freq);
    freq = hslider("pitch", spn.C3, spn.C2, spn.C6, 0.001) : ba.midikey2hz;
    pluckPosition = hslider("pos", 0.5, 0, 1, 0.01); // pluck position (0-1) (1 is on the bridge)
    mute = hslider("mute", 1, 0, 1, 0.001); // mute coefficient (1 for no mute and 0 for instant mute)
    pluckGain = hslider("gain", 1, 0, 1, 0.001); // pluck gain
    trigger = button("gate");
};
