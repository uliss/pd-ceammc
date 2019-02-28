os = library("oscillators.lib");
ba = library("basics.lib");
si = library("signals.lib");

phase = hslider("phase", 0, 0, 1.0, 0.001) : si.smoo;
saw4(freq) = os.sawNp(4, freq, phase);

process(freq) = ba.if(freq == 0, 0, freq : saw4);
