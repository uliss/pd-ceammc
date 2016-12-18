import("misceffect.lib");

thresh= hslider("thresh",-90,0,-40,0.1);
atk = vslider("atk",0,5,.1,0.1);
rel = vslider("rel",0,5,.1,0.1);
hold = vslider("hold",0.1,5,.1,0.1);

process =  _,_ : gate_stereo(thresh,atk,hold,rel) : _,_;
