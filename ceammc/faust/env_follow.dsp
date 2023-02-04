declare name "env.follow";

cm  = library("ceammc.lib");
an  = library("analyzers.lib");
atk = hslider("attack [unit:ms]", 200, 1, 1000, 0.1) : cm.time_pd2faust;
rel = hslider("release [unit:ms]", 200, 1, 1000, 0.1) : cm.time_pd2faust;

process = an.amp_follower_ar(atk, rel);

	
