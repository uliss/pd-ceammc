declare name "flt.notch";

si = library("signals.lib");
// using olg version (without s), because it compiles with 2.14
fi = library("filter.lib");

freq = vslider("freq [unit:Hz]", 1000, 20, 20000, 0.1) : si.smoo;
width = vslider("width [unit:Hz]", 50, 1, 10000, 0.1) : si.smoo;

process = fi.notchw(width, freq);
