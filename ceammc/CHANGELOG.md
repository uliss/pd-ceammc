# CEAMMC external library changelog

## [Unreleased]
### Added:
- xlet tooltips added to:
  - xdac~

### Changed:
- using AtomListView instead of AtomList in BaseObject::onInlet method
- using AtomListView instead of AtomList in UI objects


## [0.9.1]
### Added:
- properties:
  - float and int property simple arithmetic added: @prop + 0.5, adds 0.5 to @prop value.
    supports +,-,* and / operators
  - bool property toggle syntax added: @bool_prop ! or @bool_prop ~ inverts boolean value
  - symbol enum properties index support added: @senum 1 - set second enum value
- mlist.flatten - simple list support added
- an.* updates:
    - an.rms~, root mean square with moving-average algorithm
- random.* updates:
    - random.int @seed property added to gen reproducible random sequences
    - random.float @seed property added to gen reproducible random sequences
    - random.gauss @seed property added to gen reproducible random sequences
    - random.linear @seed property added to gen reproducible random sequences
    - random.pw_const @seed property added to gen reproducible random sequences
    - random.pw_lin @seed property added to gen reproducible random sequences
    - random.discrete @seed property added to gen reproducible random sequences
    - random.atom (with random.a alias) added for weighted random atom generation
- flow.* updates:
    - flow.delay added: enhanced version of vanilla delay
    - flow.dup added: message repeater
    - flow.greater_eq added (with flow.>= alias)
    - flow.group second inlet added to change @by property
    - flow.match property match added, unmatched outlet added
    - flow.select added
    - flow.space added to space in time messages received in the same logical time
    - flow.stack added
    - flow.pipe added
    - flow.ring added
    - flow.route can output message with matched selector using * prefix
    - flow.split any message support added
    - flow.reject alias added: flow.!
- math new obejcts:
    - math.nan~ added (mainly for testing purposes)
    - math.inf~ added (mainly for testing purposes)
    - math.sync_rshift (with >>' and math.>>' aliases)
    - math.sync_lshift (with <<' and math.<<' aliases)
- array.* updates:
    - array.play~ added (with array.p~ alias)
- env.* updates:
    - env.smooth~ play method added
- conv.* updates:
    - dbfs->amp~ converter added
    - conv.sec2bpm (with alias sec->bpm) converter added
    - conv.ms2bpm (with alias ms->bpm) converter added
    - conv.degree2key (with degree->key alias) added
    - conv.int2bits (with int->bits alias) added
    - symbol2intlist (with sym->ilist alias) converter added
    - symbol2any (with sym->any alias) added
    - conv.hex2int (with hex->int alias) added
    - @positive property added to car->pol for output in [0..2π) range
- fx.* updates:
    - freeze added to fx.freeverb~, fx.freeverb2~ and fx.zita_rev1~ (via @freeze property)
    - fx.pitchshift_s~ added (signal version of fx.pitchshift~)
- flt.* updates:
    - flt.ff_comb~ added (feed forward comb filter)
    - flt.fb_comb~ added (feedback comb filter)
- midi.* updates:
    - midi.vramp added
    - midi.tuning added
- new category for sequences: seq.*
    - seq.bangs sequencer (with seq.b alias) added into new category: seq
    - seq.toggles sequencer (with seq.t alias) added
    - seq.nbangs added (with seq.nb alias)
    - seq.phasor added (control rate saw generator)
    - sequencer object added (with seq alias)
    - seq.matrix added
- synth:
    - synth.dubdub~ @freq property added and 'note' method
    - synth.eguitar~ ...
    - synth.ks~ ...
    - synth.marimba~ ...
    - synth.rhodey~ @pitch property added and 'note' method
    - synth.bee3~ ...
    - synth.wurley~ ...
    - synth.sitar~ added
    - synth.russian_bell~ added
    - synth.glitch~ added
- ui.* updates:
    - ui.display alias added: ui.dt for ui.display @display_type=1
    - ui.toggle @on_value/@off_value properties added
    - @float_width property added to ui.display
- base updates:
    - log.error, log.post, log.debug, log.verbose, log.stdout, log.stderr objects added
    - sync added for multiple value synchronization without stack overflow
    - RythmTree support added: rtree->list and built-in function rtree()
    - patch.deps added - list current patch dependencies
    - ceammc.search added
    - counter object added
- proto.* updates:
    - proto.xtouch_ext - Behringer XTouch externder support added 
- noise.* updates:
    - documentation for a-chaos files added
    

### Changed:
- ext_info output format changed.
    "inlets": \["audio", "control"...] instead of inlet number
    "outlets": \["audio", "control"...] instead of outlet number
- props:
    - prop.set renamed to prop.join (with prop<- alias)
    - prop.set now is a new object with different behavior, it changes properties via internal pd calls
    - prop.set alias added: p.set
    - prop.get renamed to prop.split (with prop<- and @<- aliases)
    - prop.get biw is a new object with different bihavior, it get properties from connected object/subpatch/abstraction
- spat.pan4~ and spat.pan8~:
    - @dist property renamed to @radius
    - positional args order reversed, now is: radius, angle
    - list support added for easy connection with ui.polar
- flow.count:
    - second inlet add to reset counter by bang
    - init arg added to set start value
    - method reset removed: use bang
- flow.once:
    - second inlet add to reset opened state
    - method reset removed: use bang
    - init arg added to set init state
- flow.gate: changing property values only via prop.set object
- flow.pass:
    - second inlet added to set pass list
    - pass if first list atom contains in pass list (old behavoir: pass all lists)
- flow.reject:
   - second inlet added to set reject list
   - reject if first list atom contains in reject list (old behavoir: pass all lists)
- flow.speedlim:
   - second inlet added to set time
   - reset message should be send to second inlet
- flow.multiplex:
   - additional inlet added to choose input
   - setting property only via prop.set object
- flow.route:
   - output routed data as message
- list.^contains @subj property renamed to @value
- midi.track @events property renamed to @nevents

### Fixed:
- data.float creation arg fix
- data.int creation arg fix
- string.remove doc example fix
- function right inlet accepts any message, closes #59


### Removed:
- flow.demultiplex: @noprops property removed, object now process any input data flow.
  You should get/set properties via new prop.get/prop.set objects

## [0.9]
### Added:
- an.onset - onset analyzer for array (using aubio library)
- an.onset~ - onset analyzer for signals (using aubio library)
    - flow module updates:
    - flow.< alias added for flow.less
    - flow.<= alias added for flow.less_eq
    - flow.greater added (with flow.> alias)
    - flow.' alias added for flow.sync
- tl.t alias added for tl.toggle
- tl.b alias added for tl.bang
- duplicate method added to ui.preset and preset.storage
- random.i alias added for random.int
- random.f alias added for random.float
- preset.f alias added for preset.float
- preset.s alias added for preset.symbol
- preset.l alias added for preset.list
- metro.random added
- math module added:
  - pi alias added for math.pi and argument added [pi 2] means two-pi value
  - math.pi~ added (with pi~ alias)
  - "-x" alias added for math.neg
  - ^2 alias added for math.squared
  - math.squared~ (with math.^2~ and ^2~ aliases) added
  - math.reciprocal~ (with 1/x~ alias) added
  - math.exp2~ added
  - math.exp~ added
  - math.gamma and math.gamma~ added
  - math.lgamma and math.lgamma~ added
  - math.erf and math.erf~ added
  - math.sin~ added
  - math.asin~ added
  - math.sinh~ added
  - math.asinh~ added
  - math.cos~ added
  - math.acos~ added
  - math.cosh~ added
  - math.acosh~ added
  - math.ceil~ added
  - math.floor~ added
  - math.trunc~ added
  - math.log2~ added
  - math.log~ added
  - math.log10~ added
  - math.tan~ added
  - math.atan~ added
  - math.tanh~ added
  - math.atanh~ added
  - math.cbrt~ added
- complex math for signals added:
  - math.cabs~
  - math.carg~
  - math.cexp~
- converters module:
  - conv.lin2lin~ (with lin->lin~ and l->l~ aliases) added
  - conv.amp2dbfs~ (with amp->dbfs~ alias) added
  - conv.samp2ms (with samp->ms alias) added
  - conv.samp2ms~ (with samp->ms~ alias) added
  - conv.ms2samp (with ms->samp alias) added
  - conv.ms2samp~ (with ms->samp~ alias) added
- chaos.gbman0 chaotic generator added
- chaos.gbman0~ chaotic generator added
- chaos.std0 chaotic generator added
- chaos.std0~ chaotic generator added
- chaos.logistic alias added for noise.logistic
- noise.logistic documentation added
- list.contains @all_of, @any_of, @none_of, @sublist (default) properties added
- list.resample (with alias list.r) added
- is_data @in_list property added to search data inside list
- path.is_dir added
- flt.freqz~ (amp/phase frequency response calculator)
- snd.file load options added:
  - @gain (or @g) - to apply gain. use like: @gain 0.5 or @gain +2.4db
  - @begin (or @b) - to specify start read position in input file
  - @end (or @e) - to specify end position
  - @length (or @l, @len) - to specify read segment length (cannot be used simultaneously with @end)
  - @normalize (or @n, @norm) - to normalize file right after loading
  - @gain (or @g) - to specify applied gain in amp or db
  - @aoffset (or @aoff) - to specify write offset position in destination array
  - @resample - to specify data resampling while loading
- array.resample (with alias array.r) added. Resamples array with SoxR library
- plot.linspace~ added
- plot.logspace~ added
- plot.geomspace~ added
- plot.hist~ added (with hist~ alias)
- plot.response~ added
- ui.plot~ added
- nsig~ multichannel control to audio rate converter

### Changed:
- flow.sync bang output on unitialized value
- system.shell renamed to system.exec because it not used shell any more

### Fixed:
- ui.tab init from args fixed
- ui.menu init from args fixed
- ui.menu outputs numeric labels as numbers not as symbols as before
- flt.bpf12~ non-stability fixed (by using double precision)
- flt.bpf24~ non-stability fixed (by using double precision)
- using puredata-ceammc.png icon when running on linux
- soundtouch~ installation fixed on linux
- flt.biquad~ fixed

## [0.8]
### Added:
- conv.car2pol converter from Cartesian to Polar coordinates added (with alias car->pol)
- conv.pol2car converter from Polar to Cartesian coordinates added (with alias pol->car)
- conv.rad2phase added (with alias rad->phase)
- conv.rad2phase~ added (with alias rad->phase~)
- conv.phase2rad added (with alias phase->rad)
- conv.phase2rad~ added (with alias phase->rad~)
- canvas.dir added
- canvas.name added
- canvas.path added
- flow.pack added
- flow.sync_pack (flow.pack') added
- list->props second outlet added for non-props elements
- list.at @default property added (that outputs if element is not found)
- list.walk second outlet added (with bang after reaching list last element)
- list.separate enumeration support added
- popup menu added to ui.radio in check list mode
- @mouse_events property added to some UI object, that allows do some tricky user interaction things
- ui.rslider move added (with SHIFT, change with ALT)
- ui.hgain~ alias added to create horizontal ui.gain~
- ui.hgain2~ alias added to create horizontal ui.gain2~
- ui.gain~/ui.gain2~ @relative property added for relative input mode (by default)
- ui.gain~/ui.gain2~ slow value change added in relative mode while holding SHIFT
- ui.gain~/ui.gain2~ mouse wheel support added (with slow on SHIFT)
- ui.gain~/ui.gain2~ popup added
- ui.gain~/ui.gain2~ MIDI learn added (double-click + SHIFT)
- ui.polar added
- ui.slider slow change with SHIFT added
- ui.slider log scale support added with @scale property
- ui.knob slow value change with SHIFT added
- ui.knob log scale support added with @scale property
- ui.knob mouse wheel support added (with slow on SHIFT)
- ui.mouse_filter added to filter specified mouse event types
- ui.mouse_route added to route specified mouse event types
- popup menu added to ui.preset with read/write actions
- math sync binary operations added (all inlets are hot):
  - math.sync_add (math.+', +')
  - math.sync_sub (math.-', -')
  - math.sync_mul (math.*', *')
  - math.sync_div (math./', /')
  - math.sync_mod (math.%', %')
  - math.sync_eq (math.==', ==')
  - math.sync_ne (math.!=', !=')
  - math.sync_lt (math.<', <')
  - math.sync_le (math.<=', <=')
  - math.sync_gt (math.>', >')
  - math.sync_ge (math.>=', >=')
  - math.sync_and (math.&&', &&')
  - math.sync_or (math.||', ||')
  - math.sync_xor (math.^', ^')
- system.cursor @relative property added to output coords relative to window
- system.colorpanel moved into ceammc library and @float, @int and @hex properties are added

### Changed:
- ui.tab in single mode outputs like ui.menu: list pair - INDEX TAB_NAME
- flow.demux by default handles @index and @index? properties. To pass this values use @noprops flag
- default range for ui.slider2d now is from -1 (left) to 1 (right) and from 1 (top) to -1 (bottom)
- in ui.slider2d following properties are renamed:
  - @x_min renamed to @x_left
  - @x_max renamed to @x_right
  - @y_min renamed to @y_top
  - @y_max renamed to @y_bottom
- in ui.sliders knobs are added

### Fixed:
- ui.vrd - invalid draw in check list mode
- ui.* - various label fixes
- ui.* @label property handles space in property dialog without entering ''

## [0.7.1]
### Added:
- properties dialogs for ui.* objects are improved:
  - $0 can be used in @send and @receive
  - tooltips are shown when mouse pointer is over property label
  - foldable categories added
- outer/inner label added for almost all UI widgets
- ui.matrix read/write methods added to save/load in text format
- ui.dsp~ popup menu added
- array.hist added
- array.mean added
- array.rms added
- array.stddev added
- array.sum added
- array.sum2 added
- array.variance added
- flow.match added (regexp route)
- hw.apple_smc (system management control) added
- hw.apple_sms (sudden motion sensor) added
- hw.cpu_temp added
- hw.display added
- hw.kdb_light added
- string.match added (regexp match for symbols and strings)
- array.fill sin method added
- array.fill gauss method added
- array.fill uniform method added
- documentation added for:
  - fx.distortion1~
### Changed:
- ui.dsp~ Shift+DblClick binding removed

## [0.7]
### Added:
- array.window added
- tl.timeline added
- msg.onload added
- net.host->ip added
- system.shell added
- osc.saw4~ added (Faust os.saw4~)
- osc.sin~ added for library completeness
- lfo.+tri~ added
- lfo.+square~ added
- lfo.+pulse~ added
- lfo.+saw~ added
- lfo.saw~ added
- pan.linsig~ added
- osc.saw~ algo changed to Faust os.sawtooth
- osc.tri~ added inlet for phase reset
- osc.saw~ added inlet for phase reset
- osc.square~ added inlet for phase reset
- osc.pulse~ added inlet for phase reset
- osc.impulse~ added inlet for phase reset
- fx.flanger~ @drywet property added
- fx.greyhole~ @drywet property added
- fx.sdelay~ clear and reset messages added
- fx.greyhole~ reset message added
- fx.echo~ reset message added
- fx.zita_rev1~ reset message added
- fx.zita_rev1~ arguments mapped to @decay_low, @decay_mid
- fx.chorus~ added
- fx.bitdown~ added
- fx.vocoder~ added
- prop.declare added
- prop added
- patch.props added
- ui.gain2~ added (stereo gain)
- msg.after added (send delayed message after passed input message)
- math.cmul~ added (multiplication of complex signals)
- math.cdiv~ added (division of complex signals)
- dict.each added for mapping all dict values
- is_dict predicate object added
- dict.pass added for passing only specified keys
- dict.reject added for rejecting specified keys
- dict.from_list added (with list->dict alias)
- dict.to_list added (with dict->list alias)
- conv.list2props (with aliases: list->props, any->props) added
- live.capture~ @gate property added, reset method added
- l->l alias added for lin->lin
- soundtouch~ pitch argument added
- flt.eq10~ graphic 10-band (1 octave) equalizer added
- flt.median added
- numeric external added with bunch of objects:
  - rational, for working with rational numbers:
    - rational.!=
    - rational.==
    - rational.abs
    - rational.add
    - rational.div
    - rational.from_float
    - rational.ge
    - rational.gt
    - rational.le
    - rational.lt
    - rational.mul
    - rational.new
    - rational.pow
    - rational.reciprocal
    - rational.sub
    - rational.to_float
    - rational.to_list
    - rational.unpack
  - complex, for working with complex numbers:
    - complex.!=
    - complex.==
    - complex.abs
    - complex.add
    - complex.arg
    - complex.conj
    - complex.cos
    - complex.div
    - complex.exp
    - complex.imag
    - complex.log
    - complex.log10
    - complex.mul
    - complex.new
    - complex.norm
    - complex.polar
    - complex.pow
    - complex.real
    - complex.sin
    - complex.sqrt
    - complex.sub
    - complex.tan
    - complex.to_list
    - complex.unpack
- matrix external added, for working with matrices: matrix.!=,
    matrix.<, matrix.<=, matrix.==, matrix.>, matrix.>=,
    matrix.abs, matrix.accu, matrix.acos, matrix.acosh, matrix.add,
    matrix.all, matrix.all_col, matrix.all_row, matrix.any, matrix.any_col,
    matrix.any_row, matrix.asin, matrix.asinh, matrix.at, matrix.atan,
    matrix.atanh, matrix.clip, matrix.col, matrix.col_at, matrix.conv2, matrix.cos,
    matrix.cosh, matrix.det, matrix.diag, matrix.div, matrix.exp, matrix.exp10,
    matrix.exp2, matrix.fill, matrix.from_list, matrix.full, matrix.hist,
    matrix.identity, matrix.insert_col, matrix.insert_row, matrix.inverse,
    matrix.linspace, matrix.load, matrix.logspace, matrix.matmul, matrix.matrix,
    matrix.max, matrix.mean, matrix.mean_col, matrix.mean_row, matrix.median,
    matrix.median_col, matrix.median_row, matrix.min, matrix.mul, matrix.ncols,
    matrix.new, matrix.nrows, matrix.ones, matrix.ones_like, matrix.randn,
    matrix.randu, matrix.range, matrix.range_col, matrix.range_row, matrix.rank,
    matrix.remove_col, matrix.remove_row, matrix.replace, matrix.reshape,
    matrix.resize, matrix.row, matrix.row_at, matrix.save, matrix.shift_col,
    matrix.shift_row, matrix.sign, matrix.sin, matrix.sinh, matrix.size, matrix.solve,
    matrix.stddev, matrix.stddev_col, matrix.stddev_row, matrix.sub, matrix.swap_cols,
    matrix.swap_rows, matrix.tan, matrix.tanh, matrix.to_list, matrix.transpose,
    matrix.trunc_exp, matrix.vectorise, matrix.zeros, matrix.zeros_like


### Changed:
- data type generation support for list.gen and list.each
- flow.demultiplex - second inlet added and @noprops flag added
- ui.matrix @current_col and @current_row are made unsaved properties
- \[ceammc\] object prints all library objects to Pd window
- you can request several properties in one message, line [@prop1? @prop2?(,
    the output is: [@prop1 values... @prop2 values...]
- lfo.tri~ now is zero-mean in \[-1, 1\] range
- lfo.square~ now is zero-mean in \[-1, 1\] range
- lfo.pulse~ now is in \[-1, 1\] range
- env.ar~ now supports Envelope data type, run on bang, bang on done added
- fx.echo~ @time property renamed to @delay
- live.capture~ record message only starts record process, use stop to stop it
- fx.freeverb~ @roomsize renamed to @room
- fx.freeverb2~ @roomsize renamed to @room
- fx.echo~ @time property renamed to @delay
- flt.moog_vcf~ signal inlet used for center frequency
- ui.gain~ range cnahges from [-90,30] till [-12,12] now

### Removed:
- fx.greyhole~ 3rd information outlet removed
- vector.* objects removed, use matrix externals


## [0.6]
### Added:
- env.asr~ envelope generator added
- env.smooth~ envelope generator added
- noise.crackle~ added
- noise.lfreq0~ added
- noise.lfreq~ doc added
- lfo.pulse~ added
- osc.blit~ added
- spat.pan4~ added
- spat.pan8~ added
- synth.bee3~ added
- synth.birds~ added
- synth.church_bell~ added
- synth.eguitar~ added
- synth.ks~ (Karplus-Strong) added
- synth.marimba~ added
- synth.rhodey~ added
- synth.risset_arp~ added
- synth.shakers~ added
- synth.wurley~ added
- fx.drone_box~ added
- fx.echo~ added
- fx.sdelay~ added
- fx.wahwah~ added
- flt.eq_peak_cq~ added
- flt.moog_vcf~ added
- math.mul (math.*) added for lists
- math.div (math./) added for lists
- math.expr added
- math.polyeval added
- math.reciprocal alias added: 1/x
- conv.amp2dbfs (amp->dbfs) added
- conv.dbfs2amp (dbfs->amp) added
- conv.midi2freq (m->f) added
- gain~ added
- mix~ added
- matrix~ added
- is_data predicate added
- radio added
- xfade~ and xfade2~ added
- speech.flite added
- fluid~ (FluidSynth) added
- fx.echo~ @drywet support added
- prop.get~ (prop~>, @~>) added
- prop.get alias added (@->)
- list.^at added
- list.^contains added
- list.^search added
- list.append added
- list.contains added
- list.prepend added
- list.remove added
- list.set added
- list.deinterleave alias added: list.unzip
- list.interleave alias added: list.zip
- list.length alias added: list.size
- list.pass_if alias added: list.filter
- list.range alias added: list.minmax
- list.reduce alias added: list.foldl
- list.rotate alias added: list.>>
- list.unwrap alias added: list.rldecode
- list.wrap alias added: list.rlencode
- list.stretch doc added
- local.dict added
- local.mlist added
- local.set added
- global.mlist added
- global.dict added
- global.set added
- data.int added
- data.float added
- mlist.flatten added
- flow.append added
- flow.interval added
- flow.multiplex2~ (flow.mux2~, mux2~) added
- flow.multiplex~ (flow.mux~, mux~) added
- flow.once (once) added
- flow.route added
- ui.aview added
- Dict (dictionary) data type added :
    with syntax: \[key: value\]
    - dict.contains added
    - dict.keys added
    - dict.size added
    - dict.values added
- Mlist (multilist) data type added:
    with syntax: (a c b d 1 2 3)

### Changed:
- migration to Faust v2
- flt.hpf12~, flt.hpf24~, flt.lpf12~, flt.lpf24~ @q property removed
- array.vplay - bang message added for play reset and @reversed property added
- error message added on unsupported message
- flow.mux <-> flow.demux naming change
- global.int, local.int, global.float, local.float added Numeric interface message support:
    - \[set(
    - \[+(
    - \[-(
    - \[*(
    - \[/(
    - @value property
- data.set, local.set, global.set, data.dict, local.dict, global.dict added Collecton interface support:
    - \[set(
    - \[add(
    - \[remove(
    - \[clear(
    - @size, @empty properties
- data.list, local.list, global.list, data.mlist, local.mlist, global.mlist added List interface support:
    - \[set(
    - \[append(
    - \[prepend(
    - \[remove(
    - \[clear(
    - \[insert(
    - \[pop(
    - \[fill(
    - \[sort(
    - \[reverse(
    - \[shuffle(
    - @size, @empty properties

### Fixed:
- ui.keyboard fixed on glissando (lowest note played between keys)

### Removed:
- symbol.ends_with removed

## [0.5]
### Added:
- hw.arduino added
- proto.firmata added
- flow.speedlim added
- list.route added
- list.do added
- metro.seq added
- metro.pattern added
- new data type added: Envelope
- envelope object added
- env->array object added
- env->vline added
- env.tshift added
- env.tscale added
- env.vscale added
- env.mix added
- env.concat added
- fx.looper~ added
- fx.greyhole~ added
- pan.lin~ added
- pan.sqrt~ added
- pan.cos~ (pan~) added
- pan.spread~ added
- ui.dsp~ doc added
- array.vplay added
- @active_scale - property added to ui.slider
- change orientation of ui.slider, ui.radio, ui.sliders in edit mode by double click
- show/hide message type in ui.display by double click
- ui.label widget added
- ui.slider2d prop changed: migration from xy_range/offset to xy_min/max
- ui.gain~ widget added
- ui.icon widget added
- Aliases added:
    - ui.n alias added for ui.number
    - ui.n~ alias added for ui.number~
    - ui.hsl alias added for horizontal ui.slider
    - ui.vsl alias added for vertical ui.slider
    - ui.b alias added for ui.bang
    - ui.t alias added for ui.toggle
    - ui.m~ alias added for ui.meter~
    - ui.d alias added for ui.display
    - ui.rsl alias added for ui.rslider

### Changed:
- @range and @shift properties replaced with @min and @max in ui.sliders
- ui.bpfunc moved to ui.env
- ui.tab now has one outlet instead of three
- ui.rslider has one inlet instead of two
- u.rslider @minvalue renamed to @low, @maxvalue to @high
- ui.menu @hover mode removed - use ui.tab instead

### Fixed:
- list.interleave wrong inlet handling on Windows
- window typo fixed nutall => nuttall

## [0.4]
### Fixed:
- function - bang handling fixed
- list.choice - fixed hang on empty list
- list.choice @norepeat property added - for non repeated index choices
- flow.change @onrepeat function callback added
- ui.knob @shift, @offset change to @min, @max. Default value range now is [0-1]
- ui.rslider @sync property added, for output on every mouse move
- ui.number new methods added: +, -, *, /, ++, --

### Added:
- math.round~ added
- flow.tee~ added to separate audio and control signals
- flow.count added to count control stream events
- flow.multiplex added
- flow.demultiplex added
- flow.gate added
- flow.less added
- modplug~ added (using libmodplug) for playing vaious MOD formats:
    MOD, S3M, XM, IT, 669, AMF (both of them), AMS, DBM, DMF, DSM, FAR,
    MDL, MED, MTM, OKT, PTM, STM, ULT, UMX, MT2, PSM

    see more info: https://github.com/Konstanty/libmodplug
- random.linear added
- random.pw_lin added (piecewise linear distribution)
- random.pw_const added (piecewise constant distribution)
- random.discrete added
- array.set added
- array.bpm added (via SoundTouch library: https://www.surina.net/soundtouch/)
- array.stretch added (time-stretch, pitch-shift and rate-range) 
- patch.args added
- canvas.current added
- canvas.top added
- midi.prg->str added
- midi.ctl->str added
- midi.key->str added
- midi.file added
- midi.track added
- midi.event2note added
- midi.event2ctl added
- window help added
- local.list help added
- ui.menu help added
- ui.number~ help added
- ui.number help added
- ui.meter~ help added
- ui.colorpanel help added
- ui.slider help added
- ui.radio help added
- ui.rslider help added
- ui.tab help added
- ui.incdec help added
- ui.link help added
- ui.preset help added
- list.split help added
- symbol.num_compare help added
- preset.float added
- preset.symbol added
- preset.list added
- preset.storage added

### Changed:
- random.gauss doc update
- conv.str2sec - handling of strings added (only symbols were supported)
- conv.sec2str - by default outputs string instead of symbol. Flag @symbol added for old behaivor.

## [0.3]
### Fixed:
- array.phase->sample fix with lists argument
- array.phase->sample clip phase to [0, 1] range
- ui.spectroscope~ crash fix and view improvements
- noise.* fix on compat library

### Added:
- array.phase->sample help file added
- array.sample->phase help file added
- array.each added
- array.fill added
- array.minmax added
- array.copy added
- array.do added
- list.slice added
- flow.split added
- function (func) added
- function.call (func.call) added
- math.gcd (Greatest Common Divisor) added
- math.lcm (Least Common Multiple) added
- string support added and objects:
    - string (str)
    - string2symbol (str->sym)
    - string.format (str.format)
    - string.join (str.join)
    - string.split (str.split)
    - string.remove (str.remove)
    - string.replace (str.replace)
    - string.ends_with (str.ends_with)
    - string.contains (str.contains)
    - string.starts_with (str.starts_with)
    - string.equal (str.equal)
    - string.substr (str.substr)
- Set data type added and objects:
    - data.set
    - data.set2list (set->list)
    - set.size
    - set.contains
    - set.union
    - set.intersect
    - set.diff
    - set.symdiff
    - set.equal
- list.seq uses only positive step. To get 6 4 2 use [list.seq 6 0 2]
- list.rotate alias added [list.<<]
- list.histogram added
- list.enumerate added
- list.pass_if added for list filtration

### Changed:
- pass.if renamed to flow.pass_if (with pass_if alias)
- reject.if renamed to flow.reject_if (with reject_if alias)
- pass.this renamed to flow.pass (with pass alias)
- reject.this renamed to flow.reject (with reject alias)
- pass.changed renamed to flow.change
- list.seq @closed property added for closed interval
- list.at multiple arguments support added
- list.unpack migrated list.separate - to split into sepatate atoms and output them sequentially
- list.unpack behavior now is similar to basic [unpack] - to unpack to several outlets
