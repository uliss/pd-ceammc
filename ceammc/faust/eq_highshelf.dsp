import("filter.lib");
import("ceammc.lib");

freq = hslider("freq", 1000, 20, 20000, 0.1);
gain = vslider("gain", 100, 10, 110, 0.1) : db_pd2faust;

process = highshelf(3, gain, freq);
