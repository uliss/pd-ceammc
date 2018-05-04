# CEAMMC external library changelog

## [Unreleased]
### Added:
- env.asr~ envelope generator added
- env.smooth~ envelope generator added
- fx.drone_box~ added
- noise.lfreq~ doc added
- noise.lfreq0~ added
- noise.crackle~ added
- flt.eq_peak_cq~ added
- lfo.pulse~ added
- spat.pan4~ added
- spat.pan8~ added
- synth.risset_arp~ added
- synth.birds~ added
- synth.marimba~ added
- synth.ks~ (Karplus-Strong) added
- synth.eguitar~ added
- synth.church_bell~ added
- synth.wurley~ added
- synth.bee3~ added
- synth.rhodey~ added
- synth.shakers~ added
- fx.wahwah~ added
- fx.echo~ added
- fx.sdelay~ added
- flow.append added
- flt.moog_vcf~ added
- alias for list.reduce: list.foldl
- math.mul (math.*) added for lists
- math.div (math./) added for lists
- math.expr added
- conv.amp2dbfs (amp->dbfs) added
- conv.dbfs2amp (dbfs->amp) added
- gain~ added
- speech.flite added
- fluid~ (FluidSynth) added
- prop.get~ (prop~>, @~>) added

### Changed:
- migration to Faust v2
- flt.hpf12~, flt.hpf24~, flt.lpf12~, flt.lpf24~ @q property removed

### Fixed:
- ui.keyboard fixed on glissando (lowest note played between keys)

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
