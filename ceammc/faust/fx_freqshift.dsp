declare name "fx.freqshift";
ma = library("maths.lib");

f2smp(freq) = (ma.SR, freq : /);

phasor(smp) =  1 :+~_: _, smp:fmod : _, smp:/;

unit(v1) =  (_ <: *(v1) , _'' :  - ) : + ~ (_', v1 :  *);

filters = _ <: _,_' :( unit(0.161758): unit(.733029) : unit (.94535) : unit(.990598)  ),  ( unit(.479401): unit(.876218) : unit (.976599) : unit(.9975) ) ;

cmpl_osc (freq)= f2smp(freq) : phasor : _, 6.2831853 : *<: sin,cos; //sinosc( freq ), cososc( freq );

cmpl_mul(in1,in2,in3,in4) =  in1*(in3), in2*(in4) ; 

process =  _,_ : (filters, cmpl_osc): cmpl_mul <: +,-;
