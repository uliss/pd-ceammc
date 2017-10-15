import("spat.lib");

angle = vslider("angle",0,360,10,0.1);
dist = vslider("dist",0,1,10,0.1);

process =  _: spat(4,angle/(360),dist) : _,_,_,_;
