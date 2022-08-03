declare name "synth.metro";

ui = library("ceammc_ui.lib");
cm = library("ceammc.lib");
ba = library("basics.lib");
fi = library("filters.lib");

f0 = vslider("freq0 [unit:Hz]", 2000, 100, 10000, 0.1);
f1 = vslider("freq1 [unit:Hz]", 1000, 100, 10000, 0.1);
f2 = vslider("freq2 [unit:Hz]", 1500, 100, 10000, 0.1);

db0 = vslider("gain0 [unit:db]", 3, -60, 12, 0.1) +3 : ba.db2linear;
db1 = vslider("gain1 [unit:db]", 0, -60, 12, 0.1) +3 : ba.db2linear;
db2 = vslider("gain2 [unit:db]", 3, -60, 12, 0.1) +3 : ba.db2linear;

Q0 = vslider("q0", 30,  3, 300, 0.1);
Q1 = vslider("q1", 10,  3, 300, 0.1);
Q2 = vslider("q2", 300, 3, 300, 0.1);

b0 = button("b0") : ba.impulsify : fi.resonbp(f0, Q0, db0) : fi.lowpass(2, f0);
b1 = button("b1") : ba.impulsify : fi.resonbp(f1, Q1, db1) : fi.lowpass(2, f1);
b2 = button("b2") : ba.impulsify : fi.resonbp(f2, Q2, db2) : fi.lowpass(2, f2);

process = b0 , b1 , b2 :> _ ;
