# CEAMMC external library changelog

## [Unreleased]
### Fixed:
- function - bang handling fixed
- list.choice - fixed hang on empty list
- list.choice @norepeat property added - for non repeated index choices
- flow.change @onrepeat function callback added

### Added:
- math.round~ added
- flow.tee~ added to separate audio and control signals
- flow.count added to count control stream events
- flow.multiplex added
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
