%%{
    machine numeric_common;

    action num_sign_done { num_.sign = (fc=='-') ? -1 : 1; }

    action num_bin_digit { (num_.ival <<= 1) |= (fc=='1'); }
    action num_bin_done  { cat_ = CAT_NUMBER; type_ = TYPE_BIN; }

    action num_int_digit { (num_.ival *= 10) += (fc-'0'); }
    action num_int_done  { num_.ival *= num_.sign; cat_ = CAT_NUMBER; type_ = TYPE_INT; }

    action num_hex_digit { (num_.ival <<= 4) |= xchar2digit(fc); }
    action num_hex_done  { cat_ = CAT_NUMBER; type_ = TYPE_HEX; }

    action num_ratio_num  { (num_.inum *= 10) += (fc-'0'); }
    action num_ratio_den  { (num_.iden *= 10) += (fc-'0'); }
    action num_ratio_done { num_.inum *= num_.sign; cat_ = CAT_NUMBER; type_ = TYPE_RATIO; }

    action num_float_done {
        cat_ = CAT_NUMBER;
        type_ = TYPE_FLOAT;
        num_.fval = num_.ival + num_.sign * (double(num_.inum)/num_.iden);
    }

    action num_float_inf {
        cat_ = CAT_NUMBER;
        type_ = TYPE_INF;
    }

    # no actions
    num_sign = '-' | '+';
    num_b = [01]+;
    num_i = '0' | ([1-9] [0-9]*);
    num_x = [0-9A-Fa-f]+;
    num_bpre = '0b';
    num_xpre = '0x';
    num_fp   = '.';
    num_slash = '/';
    num_inf = 'inf';

    # actions
    num_bin   = (num_bpre (num_b $num_bin_digit)) %num_bin_done;
    num_int   = ((num_sign @num_sign_done)? num_i $num_int_digit) %num_int_done;
    num_hex   = (num_xpre (num_x $num_hex_digit)) %num_hex_done;

    action num_fadd_frac {(num_.inum *= 10) += (fc - '0'); num_.iden *= 10;}

    num_ratio = ((num_sign @num_sign_done)?
                 num_i $num_ratio_num
                 num_slash
                 num_i $num_ratio_den)
                %num_ratio_done;

    num_frac = '.' @{num_.inum = 0; num_.iden = 1;} ([0-9]+ $num_fadd_frac);
    num_float = (num_int num_frac) %num_float_done;

    num_infinity = (num_sign? num_inf) %num_float_inf;

    num_percent = (
                  (num_float %num_float_done | (num_int %num_int_done) %{num_.fval = num_.ival;})
                  '_'?
                  '%')
                  %{type_ = TYPE_PERCENT;};
    num_phase =   (
                  (num_float %num_float_done | (num_int %num_int_done) %{num_.fval = num_.ival;})
                  ('*' | 'phase' | '_phase')
                  )
                  %{type_ = TYPE_PHASE;};


    number = num_float | num_int | num_bin | num_hex | num_ratio | num_infinity | num_percent | num_phase;
}%%
