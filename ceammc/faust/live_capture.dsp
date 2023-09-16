declare name 		"live.capture";
declare version 	"1.3";
declare author 		"Grame";
declare author 		"Serge Poltavski";
declare license 	"BSD";
declare copyright 	"(c)GRAME 2006";

//-------------------------------------------------
// Capture : record up to 32s of sound and
// playback the recorded sound in loop
//-------------------------------------------------

import("stdfaust.lib");
ui = library("ceammc_ui.lib");

process	= capture * gain : fi.dcblocker with {
    env     = en.adsr(a, d, s, r, I);
    capture = *(env) : (+ : de.delay(TOTAL * ma.SR, (D/div)-1)) ~ *(1.0-env);

    TOTAL = 32;
    B = checkbox("gate");   // Capture sound while pressed
    I = int(B);             // convert button signal from float to integer
    R = (I-I') <= 0;        // Reset capture when button is pressed
    D = (+(I):*(R))~_;      // Compute capture duration while button is pressed: 0..NNNN0..MMM

    div = hslider(".div", 1, 1, 8, 1) : int;
    gain = hslider("gain [unit:db]", 0, -60, 12, 0.001) : ba.db2linear : si.smoo;
    a = ui.attack(50);
    d = ui.decay(0);
    s = ui.sustain(100);
    r = ui.release(50);
};
