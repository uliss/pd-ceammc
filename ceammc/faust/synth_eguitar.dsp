declare name "synth.eguitar";

ui = library("ceammc_ui.lib");
cm = library("ceammc.lib");
spn = library("spn.lib");
pm = library("physmodels.lib");
ba = library("basics.lib");

process = pm.elecGuitar(len, pluckPosition, 1.0 - mute, pluckGain, trigger) : _ with {
    len = pm.f2l(freq);
    freq = hslider("pitch", spn.C3, spn.C2, spn.C6, 0.001) : ba.midikey2hz;
    pluckPosition = hslider("pos", 0.5, 0, 1, 0.01); // pluck position (0-1) (1 is on the bridge)
    mute = hslider("mute", 0, 0, 1, 0.001); // mute coefficient (0 for no mute and 1 for instant mute)
    pluckGain = hslider("gain", 1, 0, 1, 0.001); // pluck gain
    trigger = button("gate");
    amp = trigger : cm.clip(0, 1);
};
