os = library("oscillators.lib");
ba = library("basics.lib");

saw(freq) = os.sawtooth(freq);

process(freq) = ba.if(freq == 0, 0, freq : saw);
