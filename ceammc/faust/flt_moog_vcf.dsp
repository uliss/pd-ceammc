si = library("signals.lib");
ve = library("vaeffects.lib");

res = vslider("res", 0.4, 0, 1, 0.001) : si.smoo;
freq = vslider("freq [unit:Hz]", 2000, 20, 20000, 0.1) : si.smoo;

process = ve.moog_vcf_2bn(res, freq);
