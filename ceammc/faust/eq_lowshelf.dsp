import("filter.lib");

freq = hslider("freq",1000,20,20000,0.1);
gain = vslider("gain",0,-90,10,0.1);

process = lowshelf(3,gain,freq);
