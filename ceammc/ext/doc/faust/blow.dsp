import("stdfaust.lib");

fluteModel_ui(pressure) = pm.fluteModel(len, mouth_pos, pressure) * gain
with{
    len = hslider("tube_len",10,0.01,20,0.01) : si.smoo;
    mouth_pos = hslider("mouth_pos",0.5,0,1,0.01) : si.smoo;
    gain  = hslider("gain",0.5,0,1,0.01);
};

process = pm.blower_ui : fluteModel_ui : ef.cubicnl(0.5, 0) : fi.highpass(3, 10) <: re.dattorro_rev_default;
