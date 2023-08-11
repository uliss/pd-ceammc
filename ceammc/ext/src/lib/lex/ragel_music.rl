%%{

machine music_common;

# tempo: Beats Per Minute
action bpm_add_int       { (bpm.ival *= 10) += (fc - '0'); }
action bpm_add_frac      { (bpm.fnum *= 10) += (fc - '0'); bpm.fden *= 10; }
action bpm_add_dot       { bpm.dur_num *= 3; bpm.dur_den *= 2; }
action bpm_init          { bpm.dur_num = 1; bpm.dur_den = 4; }
action bpm_done          { ragel_cat = CAT_UNIT; ragel_type = TYPE_BPM; }
action bpm_beat_num_init { bpm.dur_num = 0; }
action bpm_beat_num_dig  { (bpm.dur_num *= 10) += (fc - '0'); }
action bpm_beat_den_init { bpm.dur_den = 0;}
action bpm_beat_den_dig  { (bpm.dur_den *= 10) += (fc - '0'); }

bpm_frac = '.' [0-9]+ $bpm_add_frac;
bpm_natural = ([1-9] [0-9]*);
bpm_int = ('0' | bpm_natural) $bpm_add_int;
bpm_value = bpm_int bpm_frac?;
bpm_beat_num = bpm_natural >bpm_beat_num_init $bpm_beat_num_dig;
bpm_beat_den = bpm_natural >bpm_beat_den_init $bpm_beat_den_dig;

bpm_beat_dur =
    '|' ((bpm_beat_num '/' bpm_beat_den) | (bpm_beat_den %{bpm.dur_num = 1;}))
    ('.' @bpm_add_dot)?;

bpm = (
        bpm_value
        bpm_beat_dur?
        '_'?
        'bpm'?
      )
      >bpm_init
      %bpm_done;

# Scientific (standard) Pitch Notation
spn_alt = ( '#'  @{spn.alt = Alteration::SHARP; }
          | '##' @{spn.alt = Alteration::DOUBLE_SHARP; }
          | 'b'  @{spn.alt = Alteration::FLAT; }
          | 'bb' @{spn.alt = Alteration::DOUBLE_FLAT; });
spn_pitch  = ( 'A'     @{spn.note = PitchName::A; }
             | [BH]    @{spn.note = PitchName::B; }
             | 'C'     @{spn.note = PitchName::C; }
             | 'D'     @{spn.note = PitchName::D; }
             | 'E'     @{spn.note = PitchName::E; }
             | 'F'     @{spn.note = PitchName::F; }
             | 'G'     @{spn.note = PitchName::G; }
             );

spn_octave_abs =
    (
        '-1'    @{spn.oct = -1;}
        | [0-9] @{spn.oct = (fc - '0');}
    )
    %{spn.octtype = OCTAVE_ABS;};

spn_octave_rel =
    (
        ('^'{0,5} ${spn.oct++;})
        |
        ('_'{0,5} ${spn.oct--;})
    ) @{spn.octtype = OCTAVE_REL;};

spn_octave = (spn_octave_abs | spn_octave_rel) >{spn.octtype = OCTAVE_REL;};

spn_dev =
    (
        '('
        ('-'|'+')    @{spn.sign = (fc=='-') ? -1 : 1;}
        ([0-9]{1,2}) ${(spn.dev *= 10) += (fc - '0');}
        'c'          %{spn.dev *= spn.sign;}
        ')'
    );

spn = (
        spn_pitch
        spn_alt?
        spn_octave?
        spn_dev?
      )
      >{spn.alt = Alteration::NATURAL; spn.oct = 0; spn.octtype = OCTAVE_REL; spn.dev = 0;}
      %{ragel_cat = CAT_UNIT; ragel_type = TYPE_SPN;};

pitch = (spn_pitch spn_alt?)
      >{spn.alt = Alteration::NATURAL; spn.oct = 0; spn.octtype = OCTAVE_REL; spn.dev = 0;}
      %{ragel_cat = CAT_UNIT; ragel_type = TYPE_SPN;};

# notes
action note_add_dot { note.dots++; }

# duration actions
action set_dur_abs     { note.durtype = DURATION_ABS; }
action set_dur_rel     { note.durtype = DURATION_REL; }
action set_dur_num1    { note.num = 1; }
action set_dur_den1    { note.den = 1; }
action set_dur_nreps   { note.repeats = note.num; }
action reset_dur_num   { note.num = 0; }
action add_dur_num_dig { (note.num *= 10) += (fc - '0'); }
action reset_dur_den   { note.den = 0; }
action add_dur_den_dig { (note.den *= 10) += (fc - '0'); }

note_dec  = [1-9] [0-9]*;
note_dur_num = note_dec >reset_dur_num $add_dur_num_dig;
note_dur_den = note_dec >reset_dur_den $add_dur_den_dig;
note_dots = '_'? ('.'{0,3} $note_add_dot);

note_dur_rel = ('*' note_dur_num '/' note_dur_den)     %set_dur_rel
               | ('*' note_dur_num %set_dur_den1)      %set_dur_rel
               | ('/' note_dur_den %set_dur_num1)      %set_dur_rel;

note_dur_abs =
    (note_dur_num '/' note_dur_den note_dots)      %set_dur_abs
    | ((note_dur_den %set_dur_num1) note_dots)     %set_dur_abs;

dur_repeat = note_dec
             '*' %~set_dur_nreps;

dur_sequence = dur_repeat ? note_dur_abs;

note_dur_all = note_dur_rel | note_dur_abs;

note_dur_par = '|' note_dur_all;
note_rest = 'R' @{ spn.rest = 1; };

note_single = (note_rest | spn) note_dur_par?
    >{note.num = 1; note.den = 4; note.dots = 0; note.durtype = DURATION_REL;}
    %{ragel_cat = CAT_UNIT; ragel_type = TYPE_SPN;};


action note_repeat_init  { note.repeats = 0; }
action note_repeat_digit { (note.repeats *= 10) += (fc - '0'); }

note_repeat = (note_dec >note_repeat_init $note_repeat_digit) '*';
note_single_seq = note_repeat ? note_single;

note_chord_seq = note_single (['] note_single)*;
note_chord = '<' note_chord_seq '>' note_dur_par?;

note = (note_single | note_chord);

time_signature_num = ([1-9][0-9]*)  >{ ragel_ts.num = 0; }  ${ (ragel_ts.num *= 10) += (fc - '0'); };
time_signature_div = ([1-9][0-9]*)  >{ ragel_ts.div = 0; }  ${ (ragel_ts.div *= 10) += (fc - '0'); };

time_signature_dur =
    (time_signature_num '/' time_signature_div)  %{ ragel_ts.sig.push_back({ragel_ts.num, ragel_ts.div}); };

time_signature
    = '|'
    time_signature_dur
    ('+' time_signature_dur)*
    '|';


chord5 = 'maj'   %{ rg_chord_type.assign({0, 4, 7}); name = "Major triad"; }
       | 'min'   %{ rg_chord_type.assign({0, 3, 7}); name = "Minor triad"; }
       | 'aug'   %{ rg_chord_type.assign({0, 4, 8}); name = "Augmented triad";}
       | 'dim'   %{ rg_chord_type.assign({0, 3, 6}); name = "Diminished triad"; }
       | '+6'    %{ rg_chord_type.assign({0, 4, 10}); }
       | ('sus' '4'?)   %{ rg_chord_type.assign({0, 5, 7}); }
       | 'sus2'  %{ rg_chord_type.assign({0, 2, 7}); }
       ;


chord7  = '7'           %{ rg_chord_type.assign({0, 4, 7, 10}); }
        | 'maj7'        %{ rg_chord_type.assign({0, 4, 7, 11}); }
        | 'min7'        %{ rg_chord_type.assign({0, 3, 7, 10}); }
        | 'minmaj7'     %{ rg_chord_type.assign({0, 3, 7, 11}); }
        | 'aug7'        %{ rg_chord_type.assign({0, 4, 8, 10}); }
        | 'augmaj7'     %{ rg_chord_type.assign({0, 4, 8, 11}); }
        | 'dim7'        %{ rg_chord_type.assign({0, 3, 6, 9}); }
        | 'halfdim7'    %{ rg_chord_type.assign({0, 3, 6, 10}); }
        | '7-5'         %{ rg_chord_type.assign({0, 4, 6, 10}); }
        | '7sus2'       %{ rg_chord_type.assign({0, 2, 7, 10}); }
        | '7sus4'       %{ rg_chord_type.assign({0, 5, 7, 10}); }
        ;

chord9  = '9'           %{ rg_chord_type.assign({0, 4, 7, 10, 14}); }
        | 'maj9'        %{ rg_chord_type.assign({0, 4, 7, 11, 14}); }
        | 'min9'        %{ rg_chord_type.assign({0, 3, 7, 10, 14}); }
        | '7-9'         %{ rg_chord_type.assign({0, 4, 7, 10, 13}); }
        | 'minmaj9'     %{ rg_chord_type.assign({0, 3, 7, 11, 14}); }
        | 'augmaj9'     %{ rg_chord_type.assign({0, 4, 8, 11, 14}); }
        | 'aug9'        %{ rg_chord_type.assign({0, 4, 8, 10, 14}); }
        | 'halfdim9'    %{ rg_chord_type.assign({0, 3, 6, 10, 14}); }
        | 'halfdim-9'   %{ rg_chord_type.assign({0, 3, 6, 10, 13}); }
        | 'dim9'        %{ rg_chord_type.assign({0, 3, 6, 9, 14}); }
        | 'dim-9'       %{ rg_chord_type.assign({0, 3, 6, 9, 13}); }
        | '9sus4'       %{ rg_chord_type.assign({0, 5, 7, 10, 14}); }
        ;

chord11 = '11'          %{ rg_chord_type.assign({0, 4, 7, 10, 14, 17}); }
        | 'maj11'       %{ rg_chord_type.assign({0, 4, 7, 11, 14, 17}); }
        | 'min11'       %{ rg_chord_type.assign({0, 3, 7, 10, 14, 17}); }
        | 'minmaj11'    %{ rg_chord_type.assign({0, 3, 7, 11, 14, 17}); }
        | 'augmaj11'    %{ rg_chord_type.assign({0, 4, 8, 11, 14, 17}); }
        | 'aug11'       %{ rg_chord_type.assign({0, 4, 8, 10, 14, 17}); }
        | 'halfdim11'   %{ rg_chord_type.assign({0, 3, 6, 10, 14, 17}); }
        | 'dim11'       %{ rg_chord_type.assign({0, 3, 6, 9, 14, 17}); }
        ;

chord13 = '13'          %{ rg_chord_type.assign({0, 4, 7, 10, 14, 17, 21}); }
        | 'maj13'       %{ rg_chord_type.assign({0, 4, 7, 11, 14, 17, 21}); }
        | 'minmaj13'    %{ rg_chord_type.assign({0, 3, 7, 11, 14, 17, 21}); }
        | 'min13'       %{ rg_chord_type.assign({0, 3, 7, 10, 14, 17, 21}); }
        | 'augmaj13'    %{ rg_chord_type.assign({0, 4, 8, 11, 14, 17, 21}); }
        | 'aug13'       %{ rg_chord_type.assign({0, 4, 8, 10, 14, 17, 21}); }
        | 'halfdim13'   %{ rg_chord_type.assign({0, 3, 6, 10, 14, 17, 21}); }
        ;

chord_suffix = (chord5 | chord7 | chord9 | chord11 | chord13)**;

chord_name  = pitch chord_suffix;

}%%

# include "ragel_music.h"

namespace ceammc {
namespace ragel {
    const ChordSuffixList& chord_suffix_list()
    {
        static ChordSuffixList chords {
            "maj", "min", "aug", "+6", "sus4", "sus2",
            "7", "maj7", "min7", "aug7", "augmaj7", "dim7", "halfdim7", "7-5", "7sus2", "7sus4",
            "9", "maj9", "min9", "7-9", "minmaj9", "augmaj9", "aug9", "halfdim9", "halfdim-9", "dim9", "dim-9", "9sus4",
            "11", "maj11", "min11", "minmaj11", "augmaj11", "aug11", "halfdim11", "dim11",
            "13", "maj13", "min13", "minmaj13", "augmaj13", "aug13", "halfdim13",
        };
        return chords;
    }
}
}
