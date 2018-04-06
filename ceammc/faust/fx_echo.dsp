declare name "echo";
cm = library("ceammc.lib");
import("stdfaust.lib");

process = echo;
echo = ef.echo(10.0, time, feedback) with {
    time = hslider("time", 500, 10, 10000, 1) : cm.clip(10, 10000) : cm.time_pd2faust;
    feedback = hslider("feedback", 0.001, 0.001, 0.65, 0.001) : si.smoo;
};
