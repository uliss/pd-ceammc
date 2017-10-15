import("misceffect.lib");
import("ceammc.lib");

sec2samp = *(ma.SR);

win = vslider("window", 200, 10, 2000, 0.1) : time_pd2faust : sec2samp;
fade = vslider("fade", 100, 0, 1000, 0.1) : time_pd2faust : sec2samp;
SHIFT_HIGH = 60;
SHIFT_LOW = -38;

process(in, p) = in : transpose(win, fade, p : clip(SHIFT_LOW, SHIFT_HIGH)) : _;
