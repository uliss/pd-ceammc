si = library("signals.lib");
rv = library("reverbs.lib");
cm = library("ceammc.lib");

ui_roomSize = hslider("roomsize", 0.5, 0.0, 1.0, 0.001) : si.smoo;
fb2 = 0.5;
ui_damp = hslider("damp", 0.5, 0.0, 1.0, 0.001) : si.smoo;
ui_drywet = hslider("drywet", 0.33, 0.0, 1.0, 0.001) : si.smoo;

fx = _ : rv.mono_freeverb(ui_roomSize, fb2, ui_damp, 0) : _;

process = _ : cm.fx_drywet(_, fx, ui_drywet) : _;
