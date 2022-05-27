declare name "synth.kick";

import("stdfaust.lib");
cm = library("ceammc.lib");

// Port from SuperCollider (SC) to Faust of SOSkick in
// <SuperCollider>/examples/demonstrations/DrumSynths.scd
// based on a Sound-on-Sound 'synth secrets' tutorial:
// http://www.soundonsound.com/sos/jan02/articles/synthsecrets0102.asp

//https://github.com/josmithiii/faust-jos/tree/master/percussion

process = kick_mix with {
    gate  = checkbox("gate [type:float]");
    ampdb = vslider("gain [unit:db]", -20, -60, 40, 0.1);
    freq  = vslider("freq [unit:hz]", 50, 10, 5000, 0.1);
    beater_rel = vslider("release [unit:ms]", 1000, 10, 8000, 0.1) : cm.time_pd2faust;
    beater_att = vslider("attack [unit:ms]", 10, 1, 50, 0.1) : cm.time_pd2faust;
    drum_rel = beater_rel * 0.4;
    drum_att = beater_att * 0.5;

    trigger = gate>gate';
    // using gate value as amplitude: sample and hold it on gate open
    amp = gate  : cm.clip(0, 1) : ba.latch(trigger) : *(ba.db2linear(-12));

    perc(att,rel,trigger) = cm.adsr(att,0,1.0,rel,cm.envgate(att,trigger));
    pmosc(carfreq,modfreq,index) = os.oscrs(carfreq + (index*modfreq)
                                 * os.oscrs(modfreq));

    decay(n,x) = x - (x>0.0)/n;
    release(n) = + ~ decay(n);
    line(start,end,dur,trigger) = trigger : release(int(dur*float(ma.SR)))
                        : *(start-end)+end;

    lpf(freq) = fi.lowpass(3,freq);
    hpf(freq) = fi.highpass(3,freq);

    mod_freq = 5.0;
    mod_index = 5.0;
    beater_noise_level = 0.03;

    pitch_contour = line(freq*2, freq, 0.02, trigger);
    drum_osc = pmosc(pitch_contour, mod_freq, mod_index/1.3);
    drum_lpf = drum_osc : lpf(1000);
    drum_env = drum_lpf * perc(drum_att, drum_rel, trigger);

    beater_source = no.noise * beater_noise_level;
    beater_hpf = beater_source : hpf(500);
    lpf_cutoff_contour = line(6000, 500, 0.03, trigger);
    beater_lpf = beater_hpf : lpf(lpf_cutoff_contour);
    beater_env = beater_lpf * perc(beater_att, beater_rel, trigger);
    kick_mix = (drum_env + beater_env) * 2 * amp;
};
