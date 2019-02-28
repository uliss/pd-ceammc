declare name "flt.moog_vcf";

si = library("signals.lib");
ve = library("vaeffects.lib");

res = vslider("res", 0.4, 0, 1, 0.001) : si.smoo;

process(in, freq) = in : ve.moog_vcf_2bn(res, freq) : _;
