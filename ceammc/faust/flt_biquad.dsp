declare name "flt.biquad";

import ("filters.lib");

wrap(x1,b0,b1,b2,a1,a2) = b0,b1,b2,a1,a2,x1;
process = wrap : tf2;
