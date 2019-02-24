declare name "flt.eq10";

import("stdfaust.lib");
cm = library("ceammc.lib");
ui = library("ceammc_ui.lib");

eq10_filterbank(O) = chan
with{
    M = O;
    N = 10 * M; // total number of bands (highpass band, octave-bands, dc band)
    ftop = 16000;
    fc(n) = ftop * 2^(float(n-N+1)/float(M));
    chan = fi.mth_octave_filterbank5(M, ftop, N) : sum(i,N,(*(ba.db2linear(ffader(N-(i+1))))));
    ffader(i) = fader(fc(i));
    fader(i) = vslider("f%i [unit:db]", 0, -70, 10, 0.1) : si.smoo;
};

process = eq10_filterbank(1); // octave-bands = default
