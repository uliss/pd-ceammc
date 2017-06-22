# CEAMMC external library changelog

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

### Changed:
- pass.if renamed to flow.pass_if (with pass_if alias)
- reject.if renamed to flow.reject_if (with reject_if alias)
- pass.this renamed to flow.pass (with pass alias)
- reject.this renamed to flow.reject (with reject alias)
- pass.changed renamed to flow.change
- list.seq @closed property added for closed interval
