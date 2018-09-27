declare name      "BitDowner";
declare category  "Distortion";
declare author    "Viacheslav Lotsmanov (unclechu)";
declare license   "BSD";
declare copyright "(c) Viacheslav Lotsmanov, 2015";

import("filter.lib"); // smooth
import("music.lib");  // db2linear
import("math.lib");   // if

bitLimit = 16;

downbit =
    vslider("bits", bitLimit, 1, bitLimit, 0.1)
    : (2 ^ (_-1))
    ;

downsampling =
    vslider(
        "downsamp",
        1, 1, 200, 1)
    : int
    ;

// from 0 till x (if x is 5 then [0,1,2,3,4])
counter(x) = int(_)~(_ <: if(_<(x-1) , _+1 , 0));

// downsampling
dsWet(s,c) = _~(if(c == 0 , s , _));
ds(s) =
    // dry signal if downsampling disabled
    if(downsampling > 1 , dsWet(s,counter(downsampling)) , s)
    ;

hardLimit(s) = if(s>1, 1, if(s<-1, -1, s));

// bitdowning
bd = *(downbit) : floor : /(downbit) : hardLimit;

process =  bd : ds;
