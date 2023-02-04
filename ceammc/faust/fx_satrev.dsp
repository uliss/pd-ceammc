declare name "fx.satrev";

ba = library("basics.lib");
re = library("reverbs.lib");
cm = library("ceammc.lib");
ui = library("ceammc_ui.lib");

process = _ <: cm.fx_wrap2(ui.bypass, ui.drywet(0.25), (_,!) : re.satrev) : _,_;
