import("compressor.lib");

ratio = hslider("ratio",1000,20,20000,0.1);
thresh= hslider("thresh",100,1,5000,0.1);
atk = vslider("atk",0,-90,10,0.1);
rel = vslider("rel",0,-90,10,0.1);

process =  _,_ : compressor_stereo(ratio,thresh,atk,rel) : _,_;