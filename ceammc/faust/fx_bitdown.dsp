declare name      "fx.bitdown";
declare category  "Distortion";
declare author    "Viacheslav Lotsmanov (unclechu)";
declare license   "BSD";
declare copyright "(c) Viacheslav Lotsmanov, 2015";

import("stdfaust.lib");
ui = library("ceammc_ui.lib");

process =  ba.bypass1(ui.bypass, bd: ds) with {
    bitLimit = 16;
    hardLimit(s) = ba.if(s>1, 1, ba.if(s<-1, -1, s));

    // from 0 till x (if x is 5 then [0,1,2,3,4])
    counter(x) = int(_)~(_ <: ba.if(_<(x-1) , _+1 , 0));

    // bitdowning
    bd = *(downbit) : floor : /(downbit) : hardLimit;

    // downsampling
    dsWet(s,c) = _~(ba.if(c == 0 , s , _));
    ds(s) =
        // dry signal if downsampling disabled
        ba.if(downsampling > 1 , dsWet(s, counter(downsampling)) , s);

    downbit =
        vslider("bits [type:int]", bitLimit, 1, bitLimit, 1)
        : (2 ^ (_-1)) : int;

    downsampling =
        vslider(
            "downsamp [type:int]",
            1, 1, 200, 1)
        : int;
};
