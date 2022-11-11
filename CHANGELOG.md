# CEAMMC Pd distribution changelog

## [2022.11]
### Added:
- migration to Pd version 0.53.0
- disable enternals libdir and import
- **CEAMMC** external library v0.9.5 - [ceammc/CHANGELOG.md](ceammc/CHANGELOG.md)

## [2021.11]
### Added:
- disable enternal FFTease and LyonPotpourri externals cause they available via deken
- separate "double" version settings from signle
- **CEAMMC** external library v0.9.4 - [ceammc/CHANGELOG.md](ceammc/CHANGELOG.md)

## [2021.03]
### Added:
- migration to Pd version 0.51.4
- separate "double" version settings from signle
- **CEAMMC** external library v0.9.2 - [ceammc/CHANGELOG.md](ceammc/CHANGELOG.md)

## [2020.12]
### Added:
- xlet tooltips support added for ceammc objects
- coremidi driver added
- **CEAMMC** external library v0.9.1 - [ceammc/CHANGELOG.md](ceammc/CHANGELOG.md)

## [2020.02]
### Added:
- migration to Pd version 0.51 done

## [2019.10]
### Added:
- migration to Pd version 0.50 done
- **CEAMMC** external library v0.8 - [ceammc/CHANGELOG.md](ceammc/CHANGELOG.md)

### Removed:
- hcs external removed (because system.cursor and system.colorpanel are moved into ceammc library)

## [2019.04]
- migration to Pd version 0.49 done
- **CEAMMC** external library v0.7.1 - [ceammc/CHANGELOG.md](ceammc/CHANGELOG.md)

## [2019.03]
### Added:
- **CEAMMC** external library v0.7 - [ceammc/CHANGELOG.md](ceammc/CHANGELOG.md)
- \[print\] object supports datatypes

## [2018.08]
### Added:
- **CEAMMC** external library v0.6 - [ceammc/CHANGELOG.md](ceammc/CHANGELOG.md)

### Fixed:
- MIDI menu saving properties added
- win32: saving preferences fixed

## [2018.03]
### Added:
- **CEAMMC** external library v0.5 - [ceammc/CHANGELOG.md](ceammc/CHANGELOG.md)

## [2017.10]
### Added:
- **CEAMMC** external library v0.4 - [ceammc/CHANGELOG.md](ceammc/CHANGELOG.md)
- **Autotune** an autotuner for Puredata ported from the autotalented LADSPA plugin

    * [autotune/autotune~]

    https://github.com/megrimm/pd-autotuned

- **FFTease** is a collection of Max/MSP objects implementing various forms of spectral sound processing.
    These include an additive-synthesis phase vocoder, noise reduction, cross synthesis,
    and more unusual forms of spectral processing.

    * [fftease/bthresher~]
    * [fftease/burrow~]
    * [fftease/cavoc27~]
    * [fftease/cavoc~]
    * [fftease/centerring~]
    * [fftease/codepend~]
    * [fftease/cross~]
    * [fftease/dentist~]
    * [fftease/disarrain~]
    * [fftease/disarray~]
    * [fftease/drown~]
    * [fftease/enrich~]
    * [fftease/ether~]
    * [fftease/leaker~]
    * [fftease/mindwarp~]
    * [fftease/morphine~]
    * [fftease/multyq~]
    * [fftease/pileup~]
    * [fftease/pvcompand~]
    * [fftease/pvgrain~]
    * [fftease/pvharm~]
    * [fftease/pvoc~]
    * [fftease/pvtuner~]
    * [fftease/pvwarpb~]
    * [fftease/pvwarp~]
    * [fftease/reanimator~]
    * [fftease/resent~]
    * [fftease/residency_buffer~]
    * [fftease/residency~]
    * [fftease/schmear~]
    * [fftease/scrape~]
    * [fftease/shapee~]
    * [fftease/swinger~]
    * [fftease/taint~]
    * [fftease/thresher~]
    * [fftease/vacancy~]
    * [fftease/xsyn~]

    https://github.com/ericlyon/fftease3.0-32bit

- **VASP** modular – Vector assembling signal processor
    for Pure Data and Max by [declare -lib vasp]

    https://github.com/grrrr/vasp

    * [vasp]
    * [vasp.abs]
    * [vasp.cabs]
    * [vasp.ccopy]
    * [vasp.cfft]
    * [vasp.channel]
    * [vasp.channel?]
    * [vasp.channels?]
    * [vasp.check]
    * [vasp.cnoise]
    * [vasp.cnorm]
    * [vasp.copy]
    * [vasp.cosc]
    * [vasp.cpowi]
    * [vasp.cset]
    * [vasp.csqr]
    * [vasp.dif]
    * [vasp.exp]
    * [vasp.fhp]
    * [vasp.fix]
    * [vasp.flp]
    * [vasp.frames]
    * [vasp.frames*]
    * [vasp.frames+]
    * [vasp.frames/]
    * [vasp.frames?]
    * [vasp.gate]
    * [vasp.gather]
    * [vasp.imm]
    * [vasp.int]
    * [vasp.join]
    * [vasp.list]
    * [vasp.log]
    * [vasp.m]
    * [vasp.max]
    * [vasp.min]
    * [vasp.minmax]
    * [vasp.mirr]
    * [vasp.noise]
    * [vasp.nonzero]
    * [vasp.offset]
    * [vasp.offset+]
    * [vasp.offset?]
    * [vasp.osc]
    * [vasp.part]
    * [vasp.peaks]
    * [vasp.phasor]
    * [vasp.polar]
    * [vasp.pow]
    * [vasp.radio]
    * [vasp.rect]
    * [vasp.rfft]
    * [vasp.rgate]
    * [vasp.rmax]
    * [vasp.rmin]
    * [vasp.rot]
    * [vasp.rpeaks]
    * [vasp.rpow]
    * [vasp.rvalleys]
    * [vasp.set]
    * [vasp.shift]
    * [vasp.sign]
    * [vasp.size]
    * [vasp.size*]
    * [vasp.size+]
    * [vasp.size/]
    * [vasp.size?]
    * [vasp.soffset]
    * [vasp.spit]
    * [vasp.split]
    * [vasp.sqr]
    * [vasp.sqrt]
    * [vasp.ssqr]
    * [vasp.ssqrt]
    * [vasp.sync]
    * [vasp.tilt]
    * [vasp.update]
    * [vasp.valleys]
    * [vasp.vector]
    * [vasp.vectors?]
    * [vasp.window]
    * [vasp.xmirr]
    * [vasp.xrot]
    * [vasp.xshift]
    * [vasp.xtilt]
    * [vasp.xwindow]

- **Leapmotion** - Pd external for leap motion

    * [leapmotion]

    https://github.com/chikashimiyama/Pd_leapmotion

- **xsample** - collection of efficient buffer-based sampling objects for Pure Data.
    There’s the variable-speed interpolating player xgroove~,
    the index-driven xplay~ and the sample-accurate recorder xrecord~.

    * [xplay~]
    * [xrecord~]
    * [xgroove~]

    https://grrrr.org/research/software/xsample/

- **zconf** library - zeroconf networking objects for Pure data. [declare -lib zconf]

    * [zconf.browse]
    * [zconf.domains]
    * [zconf.meta]
    * [zconf.resolve]
    * [zconf.service]

    https://github.com/grrrr/zconf

- **LyonPotpourri** - a collection of externals developed for the creation and performance of computer music.

    * [lyonpotpourri/adsr~]
    * [lyonpotpourri/arrayfilt~]
    * [lyonpotpourri/bashfest~]
    * [lyonpotpourri/buffet~]
    * [lyonpotpourri/bvplay~]
    * [lyonpotpourri/cartopol~]
    * [lyonpotpourri/channel~]
    * [lyonpotpourri/chopper~]
    * [lyonpotpourri/clean_selector~]
    * [lyonpotpourri/click2bang~]
    * [lyonpotpourri/click2float~]
    * [lyonpotpourri/clickhold~]
    * [lyonpotpourri/click~]
    * [lyonpotpourri/convolver~]
    * [lyonpotpourri/counter~]
    * [lyonpotpourri/distortion~]
    * [lyonpotpourri/dmach~]
    * [lyonpotpourri/dynss~]
    * [lyonpotpourri/epluribus~]
    * [lyonpotpourri/expflam~]
    * [lyonpotpourri/flanjah~]
    * [lyonpotpourri/function~]
    * [lyonpotpourri/granola~]
    * [lyonpotpourri/granulesf~]
    * [lyonpotpourri/granule~]
    * [lyonpotpourri/greater~]
    * [lyonpotpourri/impulse~]
    * [lyonpotpourri/kbuffer~]
    * [lyonpotpourri/killdc~]
    * [lyonpotpourri/latch~]
    * [lyonpotpourri/magfreq_analysis~]
    * [lyonpotpourri/markov~]
    * [lyonpotpourri/mask~]
    * [lyonpotpourri/npan~]
    * [lyonpotpourri/oscil~]
    * [lyonpotpourri/phasemod~]
    * [lyonpotpourri/player~]
    * [lyonpotpourri/poltocar~]
    * [lyonpotpourri/pulser~]
    * [lyonpotpourri/quadpan~]
    * [lyonpotpourri/rotapan~]
    * [lyonpotpourri/rtrig~]
    * [lyonpotpourri/samm~]
    * [lyonpotpourri/sarec~]
    * [lyonpotpourri/sel~]
    * [lyonpotpourri/shoehorn~]
    * [lyonpotpourri/sigseq~]
    * [lyonpotpourri/splitbank~]
    * [lyonpotpourri/splitspec~]
    * [lyonpotpourri/squash~]
    * [lyonpotpourri/stutter~]
    * [lyonpotpourri/vdb~]
    * [lyonpotpourri/vdp~]
    * [lyonpotpourri/vecdex~]
    * [lyonpotpourri/waveshape~]
    * [lyonpotpourri/windowvec~]

    https://github.com/pd-projects/lyonpotpourri

- **Soundtouch** - using Olli Parviainen SoundTouch Audio Processing Library.
    Pitch-shifter

    * [soundtouch~]

    http://www.surina.net/soundtouch/

- **disis_munger** a realtime multichannel granulator.
    * [disis_munger~]
