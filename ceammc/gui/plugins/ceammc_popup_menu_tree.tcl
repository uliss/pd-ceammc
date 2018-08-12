{control
    {general
        {bang float symbol list int send receive}}
    {flow
        { route select spigot moses until change print swap }}
    {math
        {+ - * / == abs pow expr mod div sin cos tan atan atan2 sqrt log exp}}
    {compare
        {< > <= == >= max min moses}}
    {convert
        { mtof ftom powtodb dbtopow rmstodb dbtorms clip wrap }}
    {time
        { delay pipe metro line timer cputime }}
    {midi
        {notein noteout ctlin ctlout makenote stripnote pgmin pgmout bendin bendout touchin touchout midiin midiout midiclkin sysexin }}
    {tables
        {tabread tabread4 tabwrite soundfiler table array}}
}
{audio
    {general
        {dac~ adc~ sig~ send~ receive~ throw~ catch~ samplerate~}}
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
    {array {
        array.bpm array.copy array.do array.each array.fill array.minmax
        array.set array.stretch array.vplay
    }}
    {base {
        function function.call gain~ matrix~ metro.pattern metro.seq mix~
        radio window xfade2~ xfade~
    }}
    {conv {
        array.p2s array.s2p conv.amp2dbfs conv.bpm2hz conv.bpm2ms conv.bpm2sec
        conv.cc2amp conv.dbfs2amp conv.lin2curve conv.lin2exp conv.lin2lin
        conv.pitch2midi conv.samp2sec conv.sec2samp conv.sec2str conv.str2sec
    }}
    {data {
        data.dict data.fifo data.float data.int data.list data.mlist data.set
        data.set2list dict.contains dict.keys dict.size dict.values
        mlist.flatten set.contains set.diff set.equal set.intersect set.size
        set.symdiff set.union
    }}
    {dyn {
        dyn.comp2~ dyn.comp~ dyn.gate2~ dyn.gate~ dyn.limit2~ dyn.limit~
        dyn.softclip~
    }}
    {env {
        env.adsr~ env.ar~ env.asr~ env.concat env.follow~ env.mix env.smooth~
        env.tscale env.tshift env.vscale env2array env2vline envelope
        vline2env
    }}
    {flow {
        expand_env flow.append flow.change flow.count flow.demultiplex
        flow.demultiplex2~ flow.demultiplex~ flow.gate flow.group
        flow.interval flow.less flow.less_eq flow.multiplex flow.multiplex2~
        flow.multiplex~ flow.once flow.pass flow.pass_if flow.reject
        flow.reject_if flow.route flow.speedlim flow.split flow.sync flow.tee~
        replace
    }}
    {flt {
        flt.biquad~ flt.bpf12~ flt.bpf24~ flt.c_bpf~ flt.c_highshelf~
        flt.c_hpf~ flt.c_lowshelf~ flt.c_lpf~ flt.c_notch~ flt.c_peak~
        flt.dcblock2~ flt.dcblock~ flt.eq_peak_cq~ flt.eq_peak~ flt.highshelf~
        flt.hpf12~ flt.hpf24~ flt.lowshelf~ flt.lpf12~ flt.lpf24~
        flt.moog_vcf~ flt.notch~ flt.resonbp~
    }}
    {fx {
        fx.distortion~ fx.drone_box~ fx.echo~ fx.flanger~ fx.freeverb2~
        fx.freeverb~ fx.freqshift~ fx.greyhole~ fx.looper~ fx.pitchshift~
        fx.sdelay~ fx.wahwah~ fx.zita_rev1~
    }}
    {global {
        global.dict global.float global.int global.list global.mlist
        global.set
    }}
    {hw {
        hw.arduino
    }}
    {lfo {
        lfo.impulse~ lfo.pulse~ lfo.square~ lfo.tri~
    }}
    {list {
        list.^at list.^contains list.^search list.all_of list.any_of
        list.append list.apply_to list.at list.choice list.contains list.count
        list.count_if list.delta list.distribution list.do list.each
        list.enumerate list.equal list.first list.gen list.histogram
        list.insert list.integrator list.last list.length list.max list.mean
        list.min list.none_of list.normalize list.pass_if list.prepend
        list.product list.range list.reduce list.remove list.remove_if
        list.repack list.repeat list.resize list.reverse list.rldecode
        list.rlencode list.rotate list.route list.search list.separate
        list.seq list.set list.shift list.shuffle list.slice list.sort
        list.sort_with list.split list.stretch list.sum list.unique
        list.unpack list.unzip list.walk list.zip
    }}
    {live {
        live.capture~
    }}
    {local {
        local.dict local.float local.int local.list local.mlist local.set
    }}
    {math {
        math.abs math.acos math.acosh math.and math.approx math.asin
        math.asinh math.atan math.atanh math.cbrt math.ceil math.cos math.cosh
        math.div math.e math.exp math.exp2 math.expr math.floor math.gcd
        math.inf math.lcm math.log math.log10 math.log2 math.mul math.nan
        math.neg math.or math.pi math.polyeval math.reciprocal math.round
        math.round~ math.sign math.sin math.sinh math.sqrt math.squared
        math.tan math.tanh math.trunc
    }}
    {midi {
        midi.ctl2str midi.event2ctl midi.event2note midi.event2prg midi.file
        midi.key2str midi.prg2str midi.track
    }}
    {misc {
        click~ fluid~ modplug~ speech.flite
    }}
    {msg {
        msg
    }}
    {noise {
        noise.crackle~ noise.lfreq0~ noise.lfreq~ noise.pink~ noise.white~
    }}
    {osc {
        osc.blit~ osc.impulse~ osc.pulse~ osc.saw~ osc.sinfb~ osc.square~
        osc.tri~
    }}
    {patch {
        canvas.current canvas.top patch.args
    }}
    {path {
        path.basename path.dirname path.exists path.lsdir
    }}
    {predicates {
        is_any is_bang is_data is_even is_file is_float is_list is_odd
        is_pointer is_symbol
    }}
    {preset {
        preset.float preset.list preset.storage preset.symbol
    }}
    {property {
        prop.get prop.get~ prop.set
    }}
    {proto {
        proto.firmata
    }}
    {random {
        random.discrete random.float random.gauss random.int random.linear
        random.pw_const random.pw_lin
    }}
    {snd {
        snd.file
    }}
    {spat {
        pan.cos~ pan.lin~ pan.spread~ pan.sqrt~ spat.pan4~ spat.pan8~
    }}
    {string {
        string string.contains string.ends_with string.equal string.format
        string.join string.length string.remove string.replace string.split
        string.starts_with string.substr string2symbol
    }}
    {symbol {
        symbol.equal symbol.length symbol.num_compare
    }}
    {synth {
        synth.bee3~ synth.birds~ synth.church_bell~ synth.dubdub~
        synth.eguitar~ synth.fgrain~ synth.ks~ synth.marimba~ synth.rhodey~
        synth.risset_arp~ synth.risset_tone~ synth.shakers~ synth.wurley~
    }}
    {system {
        system.cursor system.getenv system.hostname system.memsize
        system.memused system.screen_size
    }}
    {ui {
        tl.bang tl.cue tl.transport ui.aview ui.bang ui.colorpanel ui.display
        ui.dsp~ ui.env ui.gain~ ui.icon ui.incdec ui.keyboard ui.knob ui.label
        ui.link ui.matrix ui.menu ui.meter~ ui.number ui.number~ ui.preset
        ui.radio ui.rslider ui.scope~ ui.slider ui.slider2d ui.sliders
        ui.spectroscope~ ui.tab ui.toggle
    }}
    {vector {
        vector.distance vector.dot vector.length vector.normalize
    }}
}
{extra
    {autotune
        {autotune/autotune~}
    }
    {disis
        {disis_munger~}
    }
    {fftease
        {fftease/bthresher~ fftease/burrow~ fftease/cavoc27~ fftease/cavoc~ fftease/centerring~
        fftease/codepend~ fftease/cross~ fftease/dentist~ fftease/disarrain~ fftease/disarray~
        fftease/drown~ fftease/enrich~ fftease/ether~ fftease/leaker~ fftease/mindwarp~
        fftease/morphine~ fftease/multyq~ fftease/pileup~ fftease/pvcompand~ fftease/pvgrain~
        fftease/pvharm~ fftease/pvoc~ fftease/pvtuner~ fftease/pvwarpb~ fftease/pvwarp~
        fftease/reanimator~ fftease/resent~ fftease/residency_buffer~ fftease/residency~
        fftease/schmear~ fftease/scrape~ fftease/shapee~ fftease/swinger~ fftease/taint~
        fftease/thresher~ fftease/vacancy~ fftease/xsyn~}
    }
    {leapmotion
        {leapmotion}
    }
    {lyonpotpourri
        {lyonpotpourri/adsr~ lyonpotpourri/arrayfilt~ lyonpotpourri/bashfest~ lyonpotpourri/buffet~
        lyonpotpourri/bvplay~ lyonpotpourri/cartopol~ lyonpotpourri/channel~ lyonpotpourri/chopper~
        lyonpotpourri/clean_selector~ lyonpotpourri/click2bang~ lyonpotpourri/click2float~
        lyonpotpourri/clickhold~ lyonpotpourri/click~ lyonpotpourri/convolver~ lyonpotpourri/counter~
        lyonpotpourri/distortion~ lyonpotpourri/dmach~ lyonpotpourri/dynss~ lyonpotpourri/epluribus~
        lyonpotpourri/expflam~ lyonpotpourri/flanjah~ lyonpotpourri/function~ lyonpotpourri/granola~
        lyonpotpourri/granulesf~ lyonpotpourri/granule~ lyonpotpourri/greater~ lyonpotpourri/impulse~
        lyonpotpourri/kbuffer~ lyonpotpourri/killdc~ lyonpotpourri/latch~ lyonpotpourri/magfreq_analysis~
        lyonpotpourri/markov~ lyonpotpourri/mask~ lyonpotpourri/npan~ lyonpotpourri/oscil~
        lyonpotpourri/phasemod~ lyonpotpourri/player~ lyonpotpourri/poltocar~ lyonpotpourri/pulser~
        lyonpotpourri/quadpan~ lyonpotpourri/rotapan~ lyonpotpourri/rtrig~ lyonpotpourri/samm~
        lyonpotpourri/sarec~ lyonpotpourri/sel~ lyonpotpourri/shoehorn~
        lyonpotpourri/sigseq~ lyonpotpourri/splitbank~ lyonpotpourri/splitspec~ lyonpotpourri/squash~
        lyonpotpourri/stutter~ lyonpotpourri/vdb~ lyonpotpourri/vdp~ lyonpotpourri/vecdex~
        lyonpotpourri/waveshape~ lyonpotpourri/windowvec~}
    }
    {soundtouch
        {soundtouch~}
    }
    {vasp_library
        {vasp vasp.abs vasp.cabs vasp.ccopy vasp.cfft vasp.channel
        vasp.channel? vasp.channels? vasp.check vasp.cnoise vasp.cnorm
        vasp.copy vasp.cosc vasp.cpowi vasp.cset vasp.csqr vasp.dif
        vasp.exp vasp.fhp vasp.fix vasp.flp vasp.frames vasp.frames*
        vasp.frames+ vasp.frames/ vasp.frames? vasp.gate vasp.gather
        vasp.imm vasp.int vasp.join vasp.list vasp.log vasp.m vasp.max
        vasp.min vasp.minmax vasp.mirr vasp.noise vasp.nonzero vasp.offset
        vasp.offset+ vasp.offset? vasp.osc vasp.part vasp.peaks vasp.phasor
        vasp.polar vasp.pow vasp.radio vasp.rect vasp.rfft vasp.rgate
        vasp.rmax vasp.rmin vasp.rot vasp.rpeaks vasp.rpow vasp.rvalleys
        vasp.set vasp.shift vasp.sign vasp.size vasp.size* vasp.size+
        vasp.size/ vasp.size? vasp.soffset vasp.spit vasp.split vasp.sqr
        vasp.sqrt vasp.ssqr vasp.ssqrt vasp.sync vasp.tilt vasp.update vasp.valleys
        vasp.vector vasp.vectors? vasp.window vasp.xmirr vasp.xrot vasp.xshift vasp.xtilt
        vasp.xwindow}
    }
}
