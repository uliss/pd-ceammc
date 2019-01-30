# CEAMMC external library changelog

## [Unreleased]
### Added:
- \[print\] now supports Data types
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
