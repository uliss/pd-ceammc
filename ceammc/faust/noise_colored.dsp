declare name "noise.colored";

no = library("noises.lib");

process = no.colored_noise(N, alpha) with {
    N = 12;
    alpha = hslider("alpha", 0, -1, 1, 0.001);
};
