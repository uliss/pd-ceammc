os = library("oscillators.lib");
ba = library("basics.lib");

square = os.square;
process(freq) = ba.if(freq == 0, 0, freq : square);
