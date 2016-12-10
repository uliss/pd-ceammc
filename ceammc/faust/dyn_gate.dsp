import("misceffect.lib");

thresh= hslider("thresh",100,1,5000,0.1);
atk = vslider("atk",0,-90,10,0.1);
rel = vslider("rel",0,-90,10,0.1);
hold = vslider("hold",0,-90,10,0.1);

process =  _,_ : gate_stereo(thresh,atk,hold,rel) : _,_;
