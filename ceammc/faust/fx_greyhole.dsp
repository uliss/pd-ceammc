declare name        "fx.greyhole";
declare version     "1.0";
declare author      "Julian Parker, bug fixes by Till Bovermann";
declare license     "GPL2+";
declare copyright   "(c) Julian Parker 2013";

import("stdfaust.lib");
ui = library("ceammc_ui.lib");
cm = library("ceammc.lib");

fb = hslider("feedback",0.9,0.0,1.0,0.01):linear_interp;
depth = ((ma.SR/44100)*50*hslider("moddepth",0.1,0.0,1.0,0.001)):linear_interp;
freq = hslider("modfreq",2.0,0.0,10.0,0.01):linear_interp;
diff = hslider("diffusion",0.5,0.0,0.99,0.0001):linear_interp;
dt = min(65533,ma.SR*hslider("delaytime [unit:sec]",0.2,0.001,1.45,0.0001));
size = hslider("size",1.0,0.5,3.0,0.0001);
damp = hslider("damping", 0.0, 0.0, 0.99,0.001):linear_interp;

linear_interp = _<:(mem,_):+:*(0.5):_;

smooth_init(s,default) = *(1.0 - s) : + ~ (+(default*init(1)):*(s))
with
{
    init(value) = value - value';
};

// using exteral function to access list of prime numbers is slightly faster than an inline approach (as of 428bf3b)
prime_delays = ffunction(int primes (int), "jprev.h", "primes");

diffuser_nested(1,angle,g,scale) = si.bus(2) <: ( (si.bus(2)	:par(i,2,*(c_norm))

: (( si.bus(4) :> si.bus(2) :rotator(angle) : (
    de.fdelay1a(8192, prime_delays(size*scale):smooth_init(0.9999,prime_delays(size*scale)) -1  ),
    de.fdelay1a(8192, prime_delays(size*scale +10):smooth_init(0.9999,prime_delays(size*scale + 10)) -1  )
    ) )~par(i,2,*(-s_norm)))
    : par(i,2,mem:*(c_norm)) )
    ,
    par(i,2,*(s_norm)))
    :> si.bus(2)
    with
    {
        rotator(angle) = si.bus(2) <: (*(c),*(-s),*(s),*(c)) :(+,+) : si.bus(2)
        with{
            c = cos(angle);
            s = sin(angle);
        };
        c_norm = cos(g);
        s_norm = sin(g);
    };

    diffuser_nested(N,angle,g,scale) = si.bus(2) <: ( (si.bus(2)	:par(i,2,*(c_norm))

    : (( si.si.bus(4) :> si.bus(2) : diffuser_nested(N-1,angle,g,scale+13) : rotator(angle) : (
        de.fdelay1a(8192, prime_delays(size*scale):smooth_init(0.999,prime_delays(size*scale)) -1  ),
        de.fdelay1a(8192, prime_delays(size*scale +10):smooth_init(0.999,prime_delays(size*scale + 10)) -1  )
        ) )~par(i,2,*(-s_norm)))
        : par(i,2,mem:*(c_norm)) )
        ,
        par(i,2,*(s_norm)))
        :> si.bus(2)
        with
        {
            rotator(angle) = si.bus(2) <: (*(c),*(-s),*(s),*(c)) :(+,+) : si.bus(2)
            with{
                c = cos(angle);
                s = sin(angle);
            };
            c_norm = cos(g);
            s_norm = sin(g);
        };

// blackhole =
effect = _,_ : 	( si.bus(4) :> seq(i,3,diffuser_nested(4,ma.PI/2,(-1^i)*diff,10+19*i) ):par(i,2,si.smooth(damp)) )
                     ~( (de.fdelay4(512, 10+depth + depth*os.oscrc(freq)),de.fdelay4(512, 10+ depth + depth*os.oscrs(freq)) ) :
                        (de.sdelay(65536,44100/2,floor(dt)),de.sdelay(65536,44100/2,floor(dt))) :
                        par(i,2,*(fb)) ) : _,_;


process = _,_ : cm.fx_wrap2(ui.bypass, ui.drywet(1), effect) : _,_;
