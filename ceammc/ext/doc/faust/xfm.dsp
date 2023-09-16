import("stdfaust.lib");

process = _~xmod : fx : fi.dcblocker
with {
    mod0 = hslider("@mod0", 600, 0, 3000, 0.001) : si.smoo;
    mod1 = hslider("@mod1", 600, 0, 3000, 0.001) : si.smoo;
    mod2 = hslider("@mod2", 600, 1, 3000, 0.001) : si.smoo;
    mod3 = hslider("@mod3", 0.1, 0, 1, 0.001) : si.smoo;

    idx0 = hslider("@freq0", 532, 1, 3000, 0.001) : si.smoo;
    idx1 = hslider("@freq1", 369, 1, 3000, 0.001) : si.smoo;

    fm(f, i) = (_ * f) + i : os.oscsin;
    fm0 = fm(mod0, idx0);
    fm1 = fm(mod1, idx1);
    xmod = _ : fm0 : fm1 : _;
    fx(in) = in : ef.cubicnl(drive * mod3, off) with {
        drive = in : ba.downSample(mod2);
        off = 0.1;
    };
};
