si = library("signals.lib");
rv = library("reverbs.lib");
cm = library("ceammc.lib");

ui_roomSize = hslider("roomsize", 0.5, 0.0, 1.0, 0.001) : si.smoo;
fb2 = 0.5;
ui_damp = hslider("damp", 0.5, 0.0, 1.0, 0.001) : si.smoo;
ui_drywet = hslider("drywet", 0.33, 0.0, 1.0, 0.001) : si.smoo;

fx = + <: _,_ : rv.stereo_freeverb(ui_roomSize, fb2, ui_damp, 22);

process = _,_ : cm.fx_drywet(si.bus(2), fx, ui_drywet) : _,_;
