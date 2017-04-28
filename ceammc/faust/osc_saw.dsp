os = library("oscillators.lib");
ba = library("basics.lib");

process(freq) = ba.if(freq == 0, 0, freq : max(30) : os.saw4);
