%%{

machine units_common;
include numeric_common "ragel_numeric.rl";
include music_common "ragel_music.rl";

unit_suffixes =
    (
        ([hH]'z')          %{type_ = TYPE_HZ;}
        | ('sa' 'mp'?)     %{type_ = TYPE_SAMP;}
        | ('rad')          %{type_ = TYPE_RADIAN;}
        | ('deg')          %{type_ = TYPE_DEGREE;}
        | ('db' 'fs'?)     %{type_ = TYPE_DB;}
        | ('d' 'ay'?)      %{type_ = TYPE_DAY;}
        | ('h' 'our'?)     %{type_ = TYPE_HOUR;}
        | ('m' 'in'?)      %{type_ = TYPE_MIN;}
        | ('s' 'ec'?)      %{type_ = TYPE_SEC;}
        | ('ms' 'ec'?)     %{type_ = TYPE_MSEC;}
        | ('c' 'ent'?)     %{type_ = TYPE_CENT;}
        | ('semi' 'tone'?) %{type_ = TYPE_SEMITONE;}
        | '%'              %{type_ = TYPE_PERCENT;}
        | ('perc' 'ent'?)  %{type_ = TYPE_PERCENT;}
        | '*'              %{type_ = TYPE_PHASE;}
        | 'phase'          %{type_ = TYPE_PHASE;}
    )
    ;

unit_float = num_float | (num_int %{num_.fval = num_.ival;});
unit_with_suffix = unit_float '_'? unit_suffixes?;

action smpte_done {
    type_ = TYPE_SMPTE;
    smpte.hour *= smpte.sign;
    smpte.min *= smpte.sign;
    smpte.sec *= smpte.sign;
    smpte.frame *= smpte.sign;

    if (smpte.np == 2) {
        smpte.sec = smpte.min;
        smpte.min = smpte.hour;
        smpte.hour = 0;
    }
}

smpte_hour = ([0-5][0-9])  ${(smpte.hour *= 10) += (fc - '0');}  >{smpte.hour = 0; smpte.np++;};
smpte_min  = ([0-5][0-9])  ${(smpte.min *= 10) += (fc - '0');}   >{smpte.min = 0; smpte.np++;};
smpte_sec  = ([0-5][0-9])  ${(smpte.sec *= 10) += (fc - '0');}   >{smpte.sec = 0; smpte.np++;};
smpte_frame = ([0-9]{1,2}) ${(smpte.frame *= 10) += (fc - '0');} >{smpte.frame = 0;};

units_smpte =
    (
    (('-' | '+') @{smpte.sign = (fc=='-')?-1:1;})?
    smpte_hour
    ':' smpte_min
    (':' smpte_sec)?
    ('.' smpte_frame)?)
    >{smpte.np = 0;}
    %smpte_done;

unit_pos_end = ('$' | 'end')  %{pos_ = POSITION_END;};
unit_pos_cur = 'cur' %{pos_ = POSITION_CURRENT;};
units_pos = (unit_pos_cur | unit_pos_end);

units =
    (
        (units_pos? >{pos_ = POSITION_ABS;}
            (unit_with_suffix
            | num_percent %{type_ = TYPE_PERCENT;}
            | num_phase   %{type_ = TYPE_PHASE;}
            | num_ratio   %{type_ = TYPE_RATIO;}
            | units_smpte)
         )
        | bpm
    )
    >{type_ = TYPE_UNKNOWN; cat_ = CAT_UNKNOWN;}
    %{cat_ = CAT_UNIT;};

}%%
