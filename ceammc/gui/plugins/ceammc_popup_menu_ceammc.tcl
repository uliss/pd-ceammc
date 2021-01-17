{ceammc
    {an {
        an.onset an.onset~ an.pitchtrack~ an.rms~
    }}
    {array {
        array.bpm array.copy array.do array.each array.fill array.hist
        array.mean array.minmax array.play~ array.plot~ array.resample
        array.rms array.set array.stddev array.stretch array.sum array.sum2
        array.variance array.vplay array.window plot.geomspace~ plot.hist~
        plot.linspace~ plot.logspace~ plot.response~
    }}
    {base {
        ceammc.search function function.call gain~ logger matrix~
        metro.pattern metro.random metro.seq mix~ nsig~ obj.props radio sync
        window xdac~ xfade2~ xfade~
    }}
    {chaos {
        chaos.gbman0 chaos.gbman0~ chaos.jong chaos.logistic chaos.std0
        chaos.std0~
    }}
    {conv {
        array.p2s array.s2p conv.amp2dbfs conv.amp2dbfs~ conv.bits2bang
        conv.bits2int conv.bits2pos conv.bpm2hz conv.bpm2ms conv.bpm2sec
        conv.car2pol conv.cc2amp conv.dbfs2amp conv.dbfs2amp~ conv.degree2key
        conv.hex2int conv.int2bits conv.lin2curve conv.lin2exp conv.lin2lin
        conv.lin2lin~ conv.list2props conv.midi2freq conv.ms2bpm conv.ms2samp
        conv.ms2samp~ conv.phase2rad conv.phase2rad~ conv.pitch2midi
        conv.pol2car conv.rad2phase conv.rad2phase~ conv.samp2ms conv.samp2ms~
        conv.samp2sec conv.sec2bpm conv.sec2samp conv.sec2str conv.str2sec
    }}
    {data {
        data.dict data.fifo data.float data.int data.list data.mlist data.set
        data.set2list dict.contains dict.each dict.from_list dict.get
        dict.keys dict.pass dict.reject dict.size dict.to_list dict.values
        mlist.flatten rtree.to_list set.contains set.diff set.equal
        set.intersect set.size set.symdiff set.union
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
        expand_env flow.append flow.change flow.count flow.delay
        flow.demultiplex flow.demultiplex2~ flow.demultiplex~ flow.dup
        flow.gate flow.greater flow.greater_eq flow.group flow.interval
        flow.less flow.less_eq flow.match flow.mem flow.multiplex
        flow.multiplex2~ flow.multiplex~ flow.once flow.pack flow.pass
        flow.pass_if flow.pipe flow.queue flow.reject flow.reject_if flow.ring
        flow.route flow.select flow.space flow.speedlim flow.split flow.stack
        flow.sync flow.sync_pack flow.tee~ replace route.float
    }}
    {flt {
        flt.biquad~ flt.bpf12~ flt.bpf24~ flt.c_bpf~ flt.c_highshelf~
        flt.c_hpf~ flt.c_lowshelf~ flt.c_lpf~ flt.c_notch~ flt.c_peak~
        flt.dcblock2~ flt.dcblock~ flt.eq10~ flt.eq_peak_cq~ flt.eq_peak~
        flt.fb_comb~ flt.ff_comb~ flt.freqz~ flt.highshelf~ flt.hpf12~
        flt.hpf24~ flt.lowshelf~ flt.lpf12~ flt.lpf24~ flt.median
        flt.moog_vcf~ flt.notch~ flt.resonbp~
    }}
    {fx {
        fx.bitdown~ fx.chorus~ fx.distortion1~ fx.distortion2~ fx.distortion3~
        fx.distortion~ fx.drive~ fx.drone_box~ fx.echo~ fx.flanger~
        fx.freeverb2~ fx.freeverb~ fx.freqshift~ fx.granulator~ fx.greyhole~
        fx.looper~ fx.pitchshift_s~ fx.pitchshift~ fx.sdelay~ fx.vocoder~
        fx.wahwah~ fx.zita_rev1~
    }}
    {global {
        global.dict global.float global.int global.list global.mlist
        global.set
    }}
    {hw {
        hw.apple_smc hw.apple_sms hw.arduino hw.cpu_temp hw.display
        hw.kbd_light
    }}
    {lfo {
        lfo.+pulse~ lfo.+saw~ lfo.+square~ lfo.+tri~ lfo.impulse~ lfo.pulse~
        lfo.saw~ lfo.square~ lfo.tri~
    }}
    {list {
        list.^at list.^contains list.^search list.all_of list.any_of
        list.append list.apply_to list.at list.choice list.contains list.count
        list.count_if list.delta list.distribution list.do list.each
        list.enumerate list.equal list.first list.gen list.histogram
        list.insert list.integrator list.last list.length list.max list.mean
        list.min list.none_of list.normalize list.pass_if list.prepend
        list.product list.range list.reduce list.remove list.remove_if
        list.repack list.repeat list.resample list.resize list.reverse
        list.rldecode list.rlencode list.rotate list.route list.search
        list.separate list.seq list.set list.shift list.shuffle list.slice
        list.sort list.sort_with list.split list.stretch list.sum list.unique
        list.unpack list.unzip list.walk list.zip
    }}
    {live {
        live.capture~
    }}
    {local {
        local.dict local.float local.int local.list local.mlist local.set
    }}
    {math {
        math.abs math.abs~ math.acos math.acosh math.acosh~ math.acos~
        math.and math.approx math.asin math.asinh math.asinh~ math.asin~
        math.atan math.atanh math.atanh~ math.atan~ math.cabs~ math.carg~
        math.cbrt math.cbrt~ math.cdiv~ math.ceil math.ceil~ math.cexp~
        math.cmul~ math.cos math.cosh math.cosh~ math.cos~ math.div math.e
        math.erf math.erf~ math.exp math.exp2 math.exp2~ math.expr math.exp~
        math.floor math.floor~ math.gamma math.gamma~ math.gcd math.inf
        math.inf~ math.lcm math.lgamma math.lgamma~ math.log math.log10
        math.log10~ math.log2 math.log2~ math.log~ math.mul math.nan math.nan~
        math.neg math.or math.pi math.pi~ math.polyeval math.reciprocal
        math.reciprocal~ math.round math.round~ math.sign math.sin math.sinh
        math.sinh~ math.sin~ math.sqrt math.sqrt~ math.squared math.squared~
        math.sync_add math.sync_and math.sync_div math.sync_eq math.sync_ge
        math.sync_gt math.sync_le math.sync_lshift math.sync_lt math.sync_mod
        math.sync_mul math.sync_ne math.sync_or math.sync_rshift math.sync_sub
        math.sync_xor math.tan math.tanh math.tanh~ math.tan~ math.trunc
        math.trunc~
    }}
    {midi {
        midi.cc midi.clock midi.ctl2str midi.event2ctl midi.event2note
        midi.event2prg midi.file midi.key2str midi.prg2str midi.sysex
        midi.track midi.tuning midi.vramp
    }}
    {misc {
        click~ fluid~ modplug~ speech.flite
    }}
    {msg {
        loadexpr msg msg.after msg.onload
    }}
    {net {
        net.host2ip
    }}
    {noise {
        noise.baker noise.clifford noise.collatz noise.crackle~ noise.duffing
        noise.ginger noise.henon noise.henon_heilles noise.henon_phase
        noise.henonf noise.ikeda noise.jong noise.lfreq0~ noise.lfreq~
        noise.logistic noise.lorenz noise.lyapunov noise.navier_stokes
        noise.pink~ noise.rossler noise.stein noise.torus noise.verhulst
        noise.white~
    }}
    {osc {
        osc.blit~ osc.impulse~ osc.pulse~ osc.saw4~ osc.saw~ osc.sinfb~
        osc.sin~ osc.square~ osc.tri~
    }}
    {patch {
        canvas.current canvas.dir canvas.name canvas.path canvas.top
        patch.args patch.deps patch.tree
    }}
    {path {
        path.basename path.dirname path.exists path.is_dir path.lsdir
    }}
    {predicates {
        is_any is_bang is_data is_dict is_even is_file is_float is_list is_odd
        is_pointer is_symbol
    }}
    {preset {
        preset.float preset.list preset.storage preset.symbol
    }}
    {property {
        patch.props prop prop.declare prop.get prop.get~ prop.join prop.set
        prop.split
    }}
    {proto {
        proto.firmata proto.midi proto.sp.alpaca proto.xtouch_ext
    }}
    {random {
        random.atom random.discrete random.float random.gauss random.int
        random.linear random.pw_const random.pw_lin
    }}
    {seq {
        seq.arp seq.bangs seq.counter seq.matrix seq.nbangs seq.phasor
        seq.toggles sequencer
    }}
    {snd {
        snd.file
    }}
    {spat {
        hoa.2d.decoder~ hoa.2d.encoder~ hoa.2d.map~ hoa.2d.optim~
        hoa.2d.projector~ hoa.2d.recomposer~ hoa.2d.rotate~ hoa.2d.wider~
        hoa.@process hoa.in hoa.in~ hoa.out hoa.out~ hoa.process~ hoa.scope~
        pan.cos~ pan.linsig~ pan.lin~ pan.spread~ pan.sqrt~ spat.pan4~
        spat.pan8~ spat.zita6x8~
    }}
    {string {
        string string.contains string.ends_with string.equal string.format
        string.join string.length string.match string.remove string.replace
        string.split string.starts_with string.substr string2symbol
    }}
    {symbol {
        symbol.equal symbol.length symbol.num_compare symbol2any
        symbol2intlist
    }}
    {synth {
        synth.bee3~ synth.birds~ synth.church_bell~ synth.dubdub~ synth.dx7~
        synth.eguitar~ synth.fgrain~ synth.glitch~ synth.ks~ synth.marimba~
        synth.rhodey~ synth.risset_arp~ synth.risset_tone~ synth.russian_bell~
        synth.shakers~ synth.sitar~ synth.wurley~
    }}
    {system {
        system.colorpanel system.cursor system.exec system.getenv
        system.hostname system.memsize system.memused system.screen_size
    }}
    {tl {
        tl.bang tl.cue tl.timeline tl.toggle tl.transport
    }}
    {ui {
        ui.aview ui.bang ui.colorpanel ui.display ui.dsp~ ui.env ui.gain2~
        ui.gain~ ui.icon ui.incdec ui.keyboard ui.knob ui.label ui.link
        ui.matrix ui.menu ui.meter~ ui.midi ui.mouse_filter ui.mouse_route
        ui.number ui.number~ ui.plot~ ui.polar ui.preset ui.radio ui.rslider
        ui.scope~ ui.slider ui.slider2d ui.sliders ui.spectroscope~ ui.tab
        ui.toggle
    }}
}
