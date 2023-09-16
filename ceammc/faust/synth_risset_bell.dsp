// Risset Bell
// Ported from SuperCollider implementation by Matthew Shanley:
// http://blog.littlesecretsrecords.com/2011/09/26/risset-bell-in-supercollider/
// which came from a Pd version such as this one:
// http://crca.ucsd.edu/~msp/techniques/v0.11/book-html/node71.html

cm = library("ceammc.lib");


//--------------------------------------------------------------------------------------//
//----------------------------------------abclib----------------------------------------//
//
//-------------------------FAUST CODE AND UTILITIES FOR MIXED MUSIC---------------------//
//
//-----------------------BY ALAIN BONARDI & PAUL GOUTMANN - 2019-2021 ------------------//
//---------------------CICM - MUSIDANSE LABORATORY - PARIS 8 UNIVERSITY-----------------//
//--------------------------------------------------------------------------------------//
//
declare author "Alain Bonardi & Paul Goutmann";
declare licence "LGPLv3";
declare name "synth.risset_bell";
//
//--------------------------------------------------------------------------------------//
//RISSET'S BELL
//--------------------------------------------------------------------------------------//
//
import("stdfaust.lib");
//
//requires abcdbcontrol.dsp
//
//--------------------------------------------------------------------------------------//
//CONTROL PARAMETERS
//--------------------------------------------------------------------------------------//
//global parameters to control the features of the bell
//
dur = hslider("dur [unit:ms]", 5000, 10, 600000, 1) / 1000; //global duration in milliseconds
freq = hslider("freq [unit:Hz]", 440, 1, 10000, 0.01);
pfreq2 = hslider("pfreq", 1, 0, 10, 0.001) : si.smoo;
//----------------------------------------------------------------------------------------//
//global amplitude of the bell
//gain = hslider("gain [unit:dB]", 0, -127, 18, 0.01) : dbtogain;
//
//----------------------------------------------------------------------------------------//
//triggering of the bell
gate = button("gate");
trigger = gate>gate';
gain = gate  : cm.clip(0, 1) : ba.latch(trigger);
//
//----------------------------------------------------------------------------------------//
//BELL ENVELOPE FUNCTION
//----------------------------------------------------------------------------------------//
bellEnveloppe(attack, rdur) = _ : en.ar(0.005, bellDur) : *(envamp) <:(_, _) : * <:(_, _) : *
with {
    envamp = sqrt(sqrt(attack * 0.1));
    bellDur = rdur * dur;
};
//----------------------------------------------------------------------------------------//
//BELLOSCILLATOR2 FUNCTION WITH 2 OSCILLATORS
//----------------------------------------------------------------------------------------//
bellOscillator2(rfreq, detune) = (os.osccos(fr1), os.osccos(fr2)) : (*(a1), *(a2)) : +
with {
    fr1 = rfreq*freq+detune;
    fr2 = fr1 * pfreq2;
    test = (pfreq2 == 1.);
    a1 = 0.5*(1+test);
    a2 = 0.5*(1-test);
};
//
//----------------------------------------------------------------------------------------//
//BELLGENERATOR2 FUNCTION WITH DOUBLE SINUS AND FILTER IN PARALLEL
//----------------------------------------------------------------------------------------//
bellGenerator2(attack, rdur, rfreq, detune) = _ : bell : *(gain)
with {
    fc = rfreq * freq + detune;
    benv = bellEnveloppe(attack, rdur);
    bosc = bellOscillator2(rfreq, detune);
    bell = bosc * benv;
};
//
//----------------------------------------------------------------------------------------//
//----------------------------------------------------------------------------------------//
bell1 = bellGenerator2(1, 1, 0.56, 0.);
bell2 = bellGenerator2(0.67, 0.9, 0.56, 1.);
bell3 = bellGenerator2(1, 0.65, 0.92, 0.);
bell4 = bellGenerator2(1.8, 0.55, 0.92, 1.7);
bell5 = bellGenerator2(2.67, 0.325, 1.19, 0.);
bell6 = bellGenerator2(1.67, 0.35, 1.7, 0.);
bell7 = bellGenerator2(1.46, 0.25, 2., 0.);
bell8 = bellGenerator2(1.33, 0.2, 2.74, 0.);
bell9 = bellGenerator2(1.33, 0.15, 3., 0.);
bell10 = bellGenerator2(1., 0.1, 3.76, 0.);
bell11 = bellGenerator2(1.33, 0.075, 4.07, 0.);

rissetsbell = gate <: (bell1, bell2, bell3, bell4, bell5, bell6, bell7, bell8, bell9, bell10, bell11);

//
//--------------------------------------------------------------------------------------//
// CONVERSION DB=>LINEAR
//--------------------------------------------------------------------------------------//
dbcontrol = _ <: ((_ > -127.0), ba.db2linear) : *;
//
//--------------------------------------------------------------------------------------//
//CONTROL PARAMETER: GAIN IN DB
//--------------------------------------------------------------------------------------//
dbtogain = si.smoo : dbcontrol;
//
//
process = rissetsbell :> _;

