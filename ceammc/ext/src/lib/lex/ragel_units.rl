%%{

machine units_common;
include numeric_common "ragel_numeric.rl";
include music_common "ragel_music.rl";

action unit_int_done { ragel_num.vdouble = ragel_num.vint; }
action units_init    { ragel_type = TYPE_UNKNOWN; ragel_cat = CAT_UNKNOWN; }
action units_done    { ragel_cat = CAT_UNIT; }

unit_suffixes =
    (
        ([hH]'z')          %{ragel_type = TYPE_HZ;}
        | ('sa' 'mp'?)     %{ragel_type = TYPE_SAMP;}
        | ('rad')          %{ragel_type = TYPE_RADIAN;}
        | ('deg')          %{ragel_type = TYPE_DEGREE;}
        | ('db' 'fs'?)     %{ragel_type = TYPE_DB;}
        | ('d' 'ay'?)      %{ragel_type = TYPE_DAY;}
        | ('h' 'our'?)     %{ragel_type = TYPE_HOUR;}
        | ('m' 'in'?)      %{ragel_type = TYPE_MIN;}
        | ('s' 'ec'?)      %{ragel_type = TYPE_SEC;}
        | ('ms' 'ec'?)     %{ragel_type = TYPE_MSEC;}
        | ('c' 'ent'?)     %{ragel_type = TYPE_CENT;}
        | ('semi' 'tone'?) %{ragel_type = TYPE_SEMITONE;}
        | '%'              %{ragel_type = TYPE_PERCENT;}
        | ('perc' 'ent'?)  %{ragel_type = TYPE_PERCENT;}
        | '*'              %{ragel_type = TYPE_PHASE;}
        | 'phase'          %{ragel_type = TYPE_PHASE;}
    )
    ;

unit_float = num_float | (num_int %unit_int_done);
unit_with_suffix = unit_float '_'? unit_suffixes?;

action smpte_done {
    ragel_type = TYPE_SMPTE;
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
            | num_percent %{ragel_type = TYPE_PERCENT;}
            | num_phase   %{ragel_type = TYPE_PHASE;}
            | num_ratio   %{ragel_type = TYPE_RATIO;}
            | units_smpte)
         )
        | bpm
    )
    >units_init
    %units_done;

}%%
