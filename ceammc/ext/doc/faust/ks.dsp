declare name "Karplus Strong";
import("stdfaust.lib");

maxStringLength = 100;

my_ks(length,damping,excitation) = pm.endChain(ksChain)
with{
    // maxStringLength = pm.maxLength;
    lengthTuning = 0.5; // tuned "by hand"
    tunedLength = length-lengthTuning;
    refCoef = (1-damping)*0.2+0.8;
    refFilter = pm.ksReflexionFilter*refCoef;
    ksChain = pm.terminations(_, pm.chain(pm.in(excitation) :
    pm.stringSegment(maxStringLength,tunedLength) : pm.out),refFilter);
};

process = my_ks(length,damping,g) : fi.dcblocker : fi.dcblocker : aa.tanh1   with {
    g = pm.blower(pressure,0.5,1,5,0.25);
    pressure = hslider("pressure", 0.5, 0, 1, 0.01);
    length = hslider("length", 0.01, 0.005, maxStringLength, 0.001) : si.smoo;
    offset = hslider("offset", 0.5, 0, 1, 0.01) *0.6 +0.3;
    freq = hslider("freq", 10, 1, 20, 0.01);
    damping = no.lfnoiseN(0, freq) * 0.3 + offset;
};
