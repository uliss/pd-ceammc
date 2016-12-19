import("spat.lib");

angle = vslider("angle",0,180,10,0.1);
dist = vslider("dist",0,1,10,0.1);

process =  _ : spat(2,angle/(180),dist) : _,_;
