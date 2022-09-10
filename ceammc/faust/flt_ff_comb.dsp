declare name "flt.ff_comb";

fi = library("filters.lib");
ma = library("maths.lib");
ba = library("basics.lib");
si = library("signals.lib");

clip(x, low, high) = max(min(high, x), low);

process(in, freq) = fcomb : _ with {
    minfreq = 20;
    maxfreq = 20000;
    dfreq = clip(freq, minfreq, maxfreq);

    sr = max(8000, ma.SR);
    maxdel = ma.np2(sr / minfreq);

    del = ba.sec2samp(1 / dfreq);
    b0 = 1;
    aN = hslider("a", 1, -1, 1, 0.0001) : si.smoo;

    fcomb = in : fi.ff_fcomb(maxdel, del, b0, aN);
};
