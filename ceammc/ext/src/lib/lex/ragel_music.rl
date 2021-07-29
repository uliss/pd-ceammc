%%{

machine music_common;

# tempo: Beats Per Minute
action bpm_add_int       { (bpm.ival *= 10) += (fc - '0'); }
action bpm_add_frac      { (bpm.fnum *= 10) += (fc - '0'); bpm.fden *= 10; }
action bpm_add_dot       { bpm.dur_num *= 3; bpm.dur_den *= 2; }
action bpm_init          { bpm.dur_num = 1; bpm.dur_den = 4; }
action bpm_done          { cat_ = CAT_UNIT; type_ = TYPE_BPM; }
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
spn_alt = ( '#'  @{spn.alt = 1;}
          | '##' @{spn.alt = 2;}
          | 'b'  @{spn.alt = -1;}
          | 'bb' @{spn.alt = -2;});
spn_pitch  = ( 'A'     @{spn.note = 5;}
             | [BH]    @{spn.note = 6;}
             | [CDEFG] @{spn.note = (fc - 'C');}
             ) %{spn.pitch = (spn.note * 2) - (spn.note > 2);};

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
      >{spn.alt = 0; spn.oct = 0; spn.octtype = OCTAVE_REL; spn.dev = 0;}
      %{cat_ = CAT_UNIT; type_ = TYPE_SPN;};


# notes
action note_add_dot {
    note.dots++;
    if(note.dots == 1) {
        note.num *= 3;
        note.den *= 2;
    } else if(note.dots == 2) {
        note.num = note.num / 3 * 7;
        note.den *= 2;
    } else if(note.dots == 3) {
        note.num = note.num / 7 * 15;
        note.den *= 2;
    }
}

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
note_rest = 'R' @{note.rest = 1;};

note_single = (note_rest | spn) note_dur_par?
    >{note.num = 1; note.den = 4; note.dots = 0; note.durtype = DURATION_REL;}
    %{cat_ = CAT_UNIT; type_ = TYPE_SPN;};

note_chord_seq = note_single (['] note_single)*;
note_chord = '<' note_chord_seq '>' note_dur_par?;

note = (note_single | note_chord);

}%%
