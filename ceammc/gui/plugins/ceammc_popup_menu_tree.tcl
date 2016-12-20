{control
    {general
        {bang float symbol list int send receive}}
    {flow
        { route select spigot moses until change print swap }}
    {math
        {+ - !- * / == !/ abs pow expr mod div sin cos tan atan atan2 sqrt log exp}}
    {compare
        {< > <= == >= max min moses}}
    {convert
        { mtof ftom powtodb dbtopow rmstodb dbtorms clip wrap }}
    {time
        { delay pipe metro line timer cputime }}
    {midi
        {notein noteout ctlin ctlout makenote stripnote pgmin pgmout bendin bendout touchin touchout midiin midiout midiclkin sysexin }}
    {tables
        {tabread tabread4 tabwrite tabwrite4 soundfiler table array}}
}
{audio
    {general
        {dac~ adc~ sig~ send~ receieve~ throw~ catch~ samplerate~}}
    {math
        {+~ *~ /~ max~ min~ abs~ clip~ sqrt~ rsqrt~ pow~ log~ exp~ expr~ }}
    {filters
        {lop~ hip~ bp~ vcf~ bob~}}
    {delay
        {delwrite~ delread~ delread4~ vd~}}
    {detect
        {sigmund~ bonk~ threshold~ env~}}
    {reverb
        {rev1~ rev2~ rev3~}}
}
{ceammc
    {audio_osc
        {osc.impulse~ osc.pulse~ osc.saw~ osc.sinfb~ osc.square~ osc.tri~}
    }
    {audio_dynamics
        {dyn.comp~ dyn.comp2~ dyn.gate~ dyn.gate2~ dyn.limit~ dyn.limit2~ dyn.softclip~}}
    {audio_envelope
        {env.adsr~ env.ar~ env.follow~}}
    {audio_filters
        {flt.highshelf~ flt.lowshelf~ flt.eq_peak~ flt.dcblock~ flt.dcblock2~}}
    {audio_fx
        {fx.pitchshift~}}
    {list
        {list.apply_to list.at list.each list.length list.sort}}
    {math
        {math.abs math.acos math.acosh math.asin math.asinh math.atan math.atanh math.cbrt math.ceil math.cos math.cosh math.e math.exp math.exp2 math.floor math.inf math.lof math.log10 math.log2 math.nan math.pi math.round math.sin math.sinh math.sqrt math.tan math.tanh math.trunc}
    }
    {predicates
        {is_any is_bang is_even is_float is_list is_odd is_pointer is_symbol}
    }
    {data_pass
        {data.fifo pass.changed}
    }
}
