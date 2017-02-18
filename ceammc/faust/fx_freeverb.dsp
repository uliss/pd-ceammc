si = library("signal.lib");
rv = library("reverb.lib");
import("ceammc.lib");

roomSize = hslider("roomsize", 0.5, 0.0, 1.0, 0.001) : si.smoo;
fb2 = 0.5;
damp = hslider("damp", 0.5, 0.0, 1.0, 0.001) : si.smoo;
drywet = hslider("drywet", 0.33, 0.0, 1.0, 0.001) : si.smoo;

fx = _ : rv.mono_freeverb(roomSize, fb2, damp, 0) : _;

process = fx_dry_wet(drywet, fx);
