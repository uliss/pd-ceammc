declare name "nono.quando.h2";

declare tooltip_i0 "input A";
declare tooltip_i1 "input A";
declare tooltip_o0 "L1";
declare tooltip_o1 "L2";
declare tooltip_o2 "L3";
declare tooltip_o3 "L4";

import("stdfaust.lib");
import("piece_nono_quando.dsp");

process(in, c) = halaphon_h2(in, c);
