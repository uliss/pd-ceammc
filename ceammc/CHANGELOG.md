# CEAMMC extension library changelog

## [Unreleased]
### Fixed:
- array.phase->sample fix with lists argument
- array.phase->sample clip phase to [0, 1] range

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
- string supported added and this objects:
    - string (str)
    - string2symbol (str->sym)
    - string.format (str.format)
    - string.join (str.join)
    - string.split (str.split)
- set type added with this obbjects:
    - data.set
    - set.size
    - set.contains

### Changed:
- pass.if renamed to flow.pass_if (with pass_if alias)
- reject.if renamed to flow.reject_if (with reject_if alias)
- pass.this renamed to flow.pass (with pass alias)
- reject.this renamed to flow.reject (with reject alias)
- pass.changed renamed to flow.change
