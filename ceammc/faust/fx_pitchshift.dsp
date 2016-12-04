import("misceffect.lib");

win = vslider("window",10,10000,10,0.1);
fade = vslider("fade",10,10000,10,0.1);

process(in,p) = in : transpose(win, fade, p) : _;
