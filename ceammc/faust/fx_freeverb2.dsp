declare name "fx.freeverb2";

ba = library("basics.lib");
ma = library("maths.lib");
si = library("signals.lib");
re = library("reverbs.lib");
cm = library("ceammc.lib");
ui = library("ceammc_ui.lib");

ui_room_size = hslider("room", 0.5, 0.0, 1.0, 0.001) : si.smoo;
ui_damp = hslider("damp", 0.5, 0.0, 1.0, 0.001) : si.smoo;
ui_freeze = checkbox("freeze [type:bool]") : int;

fx = (_ * gate, _ * gate) : re.stereo_freeverb(room, fb2, damp, ui_spread) : _,_
with {
    origSR = 44100;

    freeze_time = hslider("fr_time [unit:ms][style:knob]", 100, 0.0, 1000, 0.001) : cm.time_pd2faust;
    freeze_rs = hslider("fr_room [style:knob]", 0.999, 0.0, 1.0, 0.001);
    freeze_rd = hslider("fr_damp [style:knob]", 0.0001, 0.0, 1.0, 0.001);
    ui_spread = hslider("spread", 0.5, 0.0, 1.0, 0.001) * 46 * ma.SR / origSR : int;

    smoo = si.smooth(ba.tau2pole(freeze_time));
    gate = 1 - ui_freeze : smoo;
    pass = _ * gate;

    room = ba.if(ui_freeze, freeze_rs, ui_room_size) : smoo;
    damp = ba.if(ui_freeze, freeze_rd, ui_damp) : smoo;
    fb2 = 0.5;
};

process = cm.fx_wrap2(ui.bypass, ui.drywet(0.33), fx);
