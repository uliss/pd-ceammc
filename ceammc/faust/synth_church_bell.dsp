declare name "churchBell";

ui = library("ceammc_ui.lib");
spn = library("spn.lib");
pm = library("physmodels.lib");
ba = library("basics.lib");


process = pm.churchBell(pos, cutoff, sharpness, gain, gate)
    with{
        pos = nentry("pos", 0, 0, 6, 1);
        cutoff = hslider("cutoff", 6500, 20, 20000, 1);
        sharpness = hslider("sharp", 0.5, 0.01, 5, 0.01);
        gain = hslider("gain", 1, 0, 1, 0.01);
        gate = button("gate");
    };
