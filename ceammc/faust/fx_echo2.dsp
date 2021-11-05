/* Stereo delay with feedback. */
declare name "fx.echo2";
declare author "Albert Graef";

cm = library("ceammc.lib");
ui = library("ceammc_ui.lib");
de = library("delays.lib");
ma = library("maths.lib");
si = library("signals.lib");

max_sec = 5;

echo2(dtime, level, feedback, stereo, x, y)
                = f(x,y) // the echo loop
                // mix
                : (\(u,v).(x+level*(d(u)+c(v)),
                           y+level*(d(v)+c(u))))
                // compensate for gain level
                : (/(1+level), /(1+level))
with {
    f           = g ~ (*(feedback),*(feedback));
    g(u,v,x,y)  = h(x+d(u)+c(v)), h(y+d(v)+c(u));
    h           = de.fdelay(max_sec*ma.SR, ma.SR*dtime);
    c(x)        = x*stereo;
    d(x)        = x*(1-stereo);
};

process = _,_ : cm.fx_wrap2(ui.bypass, ui.drywet(1), fx) : _,_ with {
    fx = echo2(time, 1,  feedback, stereo);
    time = hslider("delay [unit:ms]", 500, 10, max_sec * 1000, 1) : cm.time_pd2faust;
    feedback = hslider("feedback", 0.3, 0, 0.99, 0.001) : si.smoo;
    stereo = hslider("stereo", 1, 0, 1, 0.001) : si.smoo;
};
