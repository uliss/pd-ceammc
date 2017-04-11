os = library("miscoscillator.lib");
ba = library("basic.lib");

process(freq) = ba.if(freq == 0, 0, freq : max(30) : os.saw4);
