declare name "dyn.softclip";

import("stdfaust.lib");

softClip =  _ : v1 <: v2 , _ : *
with{
	v1 =  *(.1588) : min(.25) : max(-.25) : -(.25), 2: * : abs , .5 : - ;
	v2 =  _ <: (*) <: (*(2.26548) : -(5.13274) ),  _ : * : +(3.14159);
};

process = softClip;
