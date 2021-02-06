declare name "synth.harpsichord";
declare description "Nonlinear WaveGuide Commuted Harpsichord";
declare author "Romain Michon (rmichon@ccrma.stanford.edu)";
declare copyright "Romain Michon";
declare version "1.0";
declare licence "STK-4.3"; // Synthesis Tool Kit 4.3 (MIT style license);
declare description "A commuted WaveGuide Harpsichord."; 

import("instruments.lib");
spn = library("spn.lib");

//==================== GUI SPECIFICATION ================

//freq = nentry("freq [1][unit:Hz] [tooltip:Tone frequency]",440,20,20000,1);
freq = hslider("pitch", spn.C3, spn.C1, spn.C7, 0.001) : ba.midikey2hz;
gain = nentry("gain[tooltip:Gain (value between 0 and 1)]",0.8,0,1,0.01) : si.smoo;
gate = button("gate") : int;

typeModulation = nentry("modtype
[tooltip: 0=theta is modulated by the incoming signal; 1=theta is modulated by the averaged incoming signal;
2=theta is modulated by the squared incoming signal; 3=theta is modulated by a sine wave of frequency freqMod;
4=theta is modulated by a sine wave of frequency freq;]",0,0,4,1);

nonLinearity = hslider("nonlin
[tooltip:Nonlinearity factor (value between 0 and 1)]",0,0,1,0.01);

frequencyMod = hslider("modfreq
[unit:Hz][tooltip:Frequency of the sine wave for the modulation of theta (works if Modulation Type=3)]",220,20,1000,0.1);

//==================== PROCESSING ================

//----------------------- Nonlinear filter ----------------------------
//nonlinearities are created by the nonlinear passive allpass ladder filter declared in miscfilter.lib

//nonlinear filter order
nlfOrder = 6; 

//nonLinearModultor is declared in instruments.lib, it adapts allpassnn from miscfilter.lib 
//for using it with waveguide instruments
NLFM =  nonLinearModulator((nonLinearity : si.smoo),1,freq,
     typeModulation,(frequencyMod : si.smoo),nlfOrder);

//----------------------- Synthesis parameters computing and functions declaration ----------------------------

//string excitation
soundBoard = dryTapAmp*no.noise
	with{
                dryTapAmpT60 = ffunction(float getValueDryTapAmpT60(float), "harpsichord.h","");
                noteCutOffTime = freq : dryTapAmpT60*gain;
		dryTapAmp = asympT60(0.15,0,noteCutOffTime,gate);
	};	

//loopfilter is a biquad filter whose coefficients are extracted from a C++ file using the foreign function mechanism
loopFilter = fi.TF2(b0,b1,b2,a1,a2)
	   with{
		//functions are imported from the C++ file
                loopFilterb0 = ffunction(float getValueLoopFilterb0(float), "harpsichord.h","");
                loopFilterb1 = ffunction(float getValueLoopFilterb1(float), "harpsichord.h","");
                loopFilterb2 = ffunction(float getValueLoopFilterb2(float), "harpsichord.h","");
                loopFiltera1 = ffunction(float getValueLoopFiltera1(float), "harpsichord.h","");
                loopFiltera2 = ffunction(float getValueLoopFiltera2(float), "harpsichord.h","");
		//coefficients are extracted from the functions
                b0 = loopFilterb0(freq);
                b1 = loopFilterb1(freq);
                b2 = loopFilterb2(freq);
                a1 = loopFiltera1(freq);
                a2 = loopFiltera2(freq);
	   };

//delay length as a number of samples
delayLength = ma.SR/freq;

//----------------------- Algorithm implementation ----------------------------

//envelope for string loop resonance time
stringLoopGainT = gate*0.9996 + (gate<1)*releaseLoopGain(freq)*0.9 : si.smoo
		with{
                        releaseLoopGain = ffunction(float getValueReleaseLoopGain(float), "harpsichord.h","");
		};

//one string
string = (*(stringLoopGainT)+_ : de.delay(4096,delayLength) : loopFilter)~NLFM;

process = soundBoard : string : stereo : reverb
with {
    //stereoizer implement a stereo spacialisation in function of
    //the frequency period in number of samples
    stereo = _ <: _,widthdelay : stereopanner
        with {
            W = hslider("width", 0.5, 0, 1, 0.01) : si.smoo;
            A = hslider("pan", 0.6, 0, 1, 0.01) : si.smoo;
            widthdelay = de.delay(4096, W*delayLength/2);
            stereopanner = _,_ : *(1.0-A), *(A);
        };

    reverb = _,_ <: *(reverbGain),*(reverbGain),*(1 - reverbGain),*(1 - reverbGain) :
        re.zita_rev1_stereo(rdel,f1,f2,t60dc,t60m,fsmax),_,_ <: _,!,_,!,!,_,!,_ : +,+
    with {
        reverbGain = hslider("reverb_gain",0.137,0,1,0.01) : si.smoo;
        roomSize = hslider("room_size",0.72,0.01,2,0.01) : si.smoo;
        rdel = 20;
        f1 = 200;
        f2 = 6000;
        t60dc = roomSize*3;
        t60m = roomSize*2;
        fsmax = 96000;
    };

};


