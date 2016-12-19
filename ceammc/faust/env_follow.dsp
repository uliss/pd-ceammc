cm = library("ceammc.lib");
an = library("analyzer.lib");
atk = hslider("attack", 200, 1, 1000, 0.1) : cm.time_pd2faust;
rel = hslider("release", 200, 1, 1000, 0.1) : cm.time_pd2faust;

process = an.amp_follower_ar(atk, rel);
























	




