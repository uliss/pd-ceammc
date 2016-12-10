SR = fconstant(int fSamplingFreq , <math.h>);

atk = hslider("attack",200,0,1000,0.1);
rel = hslider("release",200,0,1000,0.1);

sel = _ <: (_ ,0 : > , atk : *), (_ ,0 : <= , rel : *):+;
 
s = ( ( 0.1:log ),(sel, SR : * , .0001 : *) : / ) : exp;

v =  abs;

env  = _ , _ : _ , (v <: _,_) : (_ , _ : - ) , ( _ <: _,_) :( _ , s : * ) , _ : + ;

process = env ~ _;
























	




