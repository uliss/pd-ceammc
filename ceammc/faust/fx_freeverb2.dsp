si = library("signals.lib");
re = library("reverbs.lib");
cm = library("ceammc.lib");
ui = library("ceammc_ui.lib");

ui_roomSize = hslider("room", 0.5, 0.0, 1.0, 0.001) : si.smoo;
fb2 = 0.5;
ui_damp = hslider("damp", 0.5, 0.0, 1.0, 0.001) : si.smoo;

fx = _,_ : re.stereo_freeverb(ui_roomSize, fb2, ui_damp, 0) : _,_;

process = cm.fx_wrap2(ui.bypass, ui.drywet(0.33), fx);
