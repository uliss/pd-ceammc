declare name 		"tapiir";
declare version 	"1.0";
declare author 		"Grame";
declare license 	"BSD";
declare copyright 	"(c)GRAME 2006";

//======================================================
//
// 					TAPIIR
//	  (from Maarten de Boer's Tapiir)
//
//======================================================

ma = library("maths.lib");
cm = library("ceammc.lib");
si = library("signals.lib");
ba = library("basics.lib");
de = library("delays.lib");

// mixer and matrix
//-----------------------------------------------------------

mixer(ntaps, nlines, tap, in, gain) = si.bus(ntaps + nlines) :
    par_taps, par_ins :> *(gain) with {
    par_taps = par(i, ntaps, *(tap(i)));
    par_ins = par(i, nlines, *(in(i)));
};


matrix(ntaps, nlines) = (mtxin <: tgroup("", mtx)) ~ fb with {
    MAX_DELAY_SEC = 5;
    MAX_DELAY_SAMP = ma.SR * MAX_DELAY_SEC;

    mtxin = si.bus(ntaps+nlines);
    fb = si.bus(ntaps);

    tap_fb(n, i) = vslider("tap%n.fb%i", 0, 0, 1, 0.1);
    tap_in(n, i) = vslider("tap%n.in%i", 1, 0, 1, 0.1);
    tap_gain(n) = vslider("tap%n.gain[unit:db]", 0, -60, 0, 0.1) : ba.db2linear;
    tap_del(n)  = vslider("tap%n.delay [unit:ms]", 0, 0, MAX_DELAY_SEC * 1000, 0.01) : cm.time_pd2faust : *(ma.SR);

    mix(i) = mixer(ntaps, nlines, tap_fb(i), tap_in(i), tap_gain(i));
    mtx = par(i, ntaps, mix(i) : de.fdelay(MAX_DELAY_SAMP, tap_del(i)));
};


// tapiir
//--------

tapiir(ntaps, nlines) =  si.bus(nlines) <: tmatrix <: tout
with {
    tmatrix = (matrix(ntaps, nlines), si.bus(nlines));

    out_tap(n,i) = vslider("out%n.tap%i", 0, 0, 1, 0.1);
    out_in(n,i) = vslider("out%n.in%i", 0, 0, 1, 0.1);
    out_gain(n)  = vslider("out%n.gain[unit:db]", 0, -60, 0, 0.1) : ba.db2linear;
    tout = par(i, nlines, mixer(ntaps, nlines, out_tap(i), out_in(i), out_gain(i)));
};

process = tapiir(6, 2);
