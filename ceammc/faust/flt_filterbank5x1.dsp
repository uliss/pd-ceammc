declare name "flt.fbank5x1";

fi = library("filters.lib");
si = library("signals.lib");
ba = library("basics.lib");

process = rev_outputs(filter_bank) with {
    order = 3;
    M = 1;
    N = 5; // total number of bands (highpass band, octave-bands, dc band)
    ftop = 4000;

    rev_outputs(expr) = expr <: par(i, N, ba.selector(N-(i+1), N));

    filter_bank = fi.mth_octave_filterbank(order, M, ftop, N) : par(i, N, gain(N-(i+1)));

    gain(i) = mul(i) with {
        fc(n) = ftop * 2^(float(n-N+1)/float(M));
        fader(i) = vslider("f%i [unit:db]", 0, -6, 6, 0.1) : ba.db2linear : si.smoo;
        mul(i) = _*fader(fc(i));
    };
};
