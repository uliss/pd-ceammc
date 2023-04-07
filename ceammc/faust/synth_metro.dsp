declare name "synth.metro";
import("stdfaust.lib");


process = metro with {
    metro = down_beat, on_beat, off_beat, mark :> *(gain) : co.limiter_1176_R4_mono;

    down_beat = button("down") : ba.impulsify : fi.resonlp(freq, reson, gain) with {
        freq = hslider("down.freq", 1500, 100, 5000, 0.01);
        reson = hslider("down.reson", 50, 5, 100, 0.01);
        gain = hslider("down.gain [unit:db]", 6, -12, 12, 0.01) : ba.db2linear;
    };
    on_beat = button("on") : ba.impulsify : fi.resonlp(freq, reson, gain) with {
        freq = hslider("on.freq", 1000, 100, 5000, 0.01);
        reson = hslider("on.reson", 30, 5, 100, 0.01);
        gain = hslider("on.gain [unit:db]", 6, -12, 12, 0.01) : ba.db2linear;
    };
    off_beat = button("off") : ba.impulsify : fi.resonlp(freq, reson, gain) with {
        freq = hslider("off.freq", 500, 100, 5000, 0.01);
        reson = hslider("off.reson", 10, 5, 100, 0.01);
        gain = hslider("off.gain [unit:db]", 6, -12, 12, 0.01) : ba.db2linear;
    };
    mark = button("mark") : ba.impulsify : fi.resonlp(freq, reson, gain) with {
        freq = hslider("mark.freq", 2000, 100, 5000, 0.01);
        reson = hslider("mark.reson", 500, 5, 500, 0.01);
        gain = hslider("mark.gain [unit:db]", 6, -12, 12, 0.01) : ba.db2linear;
    };
    gain = hslider("gain [unit:db]", 0, -12, 12, 0.01) : ba.db2linear;
};
