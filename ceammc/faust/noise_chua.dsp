//Chua's diode circuit
//original circuit featuring Chua's diode taken from Kurt Werner thesis
//for reference, see  Meerkotter and Scholz, "Digital Simulation of Nonlinear Circuits by Wave Digital Filter Principles"

import("stdfaust.lib");
declare name "noise.chua";

process = chua : fx,fx
with {
    //create circuit components
    c1(i) = wd.capacitor_Vout(i, hslider("C1", 5.5, 5.2, 6, 0.001)*10^(-9));
    r2(i) = wd.resistor(i, hslider("R", 1.6, 1.252, 1.7, 0.001)*10^3 : si.smoo); //resistance must be slightly higher than original document, currently unsure why.
    l3(i) = wd.inductor(i, hslider("L", 7.07, 6.5, 7.3, 0.001)*10^(-3) : si.smoo);
    c4(i) = wd.capacitor_Vout(i, hslider("C2", 49.5, 40, 60, 0.001)*10^(-9) : si.smoo);
    d1(i) = wd.u_chua(i, -500*10^-6, -800*10^-6, 1);
    // I've added a voltage injection for agitation purposes.
    // the system will always initialize with zeros, so adding an impulse will get oscillations started.
    vInject(i) = wd.seriesVoltage(i, button("gate") * 4 : ba.impulsify);

    //input tree structure
    treeChua =  d1 : (wd.parallel : (c1, (wd.series : (r2, (wd.parallel : (l3, (vInject :c4)))))));

    chua = wd.buildtree(treeChua);
    fx = fi.dcblocker : ma.tanh;
};
