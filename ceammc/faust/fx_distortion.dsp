declare name "fx.distortion";

ba = library("basics.lib");
fi = library("filters.lib");
mf = library("misceffects.lib");
si = library("signals.lib");
ui = library("ceammc_ui.lib");
cm = library("ceammc.lib");

drive = vslider("drive", 0.5, 0, 1, 0.001) : si.smoo;
offset = vslider("offset", 0, 0, 0.5, 0.001) : si.smoo;

effect = _ : mf.cubicnl_nodc(drive, offset) : fi.dcblocker : _;
process = _ : cm.fx_wrap_mono(ui.bypass, ui.drywet(1), effect) : _;
