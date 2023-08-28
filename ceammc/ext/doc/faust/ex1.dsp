declare name "example1";
import("stdfaust.lib");
amp = hslider("drywet", 0.15, 0, 1, 0.001) : si.smoo;
freq = hslider("freq", 700, 0, 1000, 0.001) : si.smoo;
mod = hslider("mod", 200, 0, 1000, 0.001) : si.smoo : os.oscsin;

process = _ : mix with {
  mix(in) = in*(1-amp),(in:fx*amp) :> _;
  fx = *(os.oscsin(freq * mod) + 0.5);
};
