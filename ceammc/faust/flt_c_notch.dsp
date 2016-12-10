import("maxmsp.lib");

freq = hslider("freq",1000,20,20000,0.1);
q = hslider("q",100,1,5000,0.1);
gain = vslider("gain",0,-90,10,0.1);

process = filtercoeff(freq,gain,q).notch;
