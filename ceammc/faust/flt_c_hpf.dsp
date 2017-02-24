import("maxmsp.lib");

freq = hslider("freq",1000,20,20000,0.1);
q = hslider("q",1,0.01,15,1);

process = filtercoeff(freq,0,q).HPF;
