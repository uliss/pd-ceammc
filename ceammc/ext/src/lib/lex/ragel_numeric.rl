%%{
    # required local result variable: ragel_num, ragel_cat, ragel_type
    # required struct fields:
    # - sign
    # - vint
    # - vdouble
    # - ratio.num
    # - ratio.den

    # use fsm::NumericData
    # from "parser_numeric.h" header

    # example:
    #
    # fsm::NumericData ragel_num;
    # AtomType ragel_type;
    # AtomCategory ragel_cat;

    machine numeric_common;

    action num_sign_done { ragel_num.sign = (fc=='-') ? -1 : 1; }

    action num_bin_digit { (ragel_num.vint <<= 1) |= (fc=='1'); }
    action num_bin_done  {
        ragel_type = TYPE_BIN;
        ragel_cat  = CAT_NUMBER;
    }

    action num_int_digit { (ragel_num.vint *= 10) += (fc-'0'); }
    action num_int_done  {
        ragel_num.vint *= ragel_num.sign;
        ragel_type = TYPE_INT;
        ragel_cat  = CAT_NUMBER;
    }

    action num_hex_digit { (ragel_num.vint <<= 4) |= xchar2digit(fc); }
    action num_hex_done  {
        ragel_type = TYPE_HEX;
        ragel_cat  = CAT_NUMBER;
    }

    action num_ratio_num  { (ragel_num.ratio.num *= 10) += (fc-'0'); }
    action num_ratio_den  { (ragel_num.ratio.den *= 10) += (fc-'0'); }
    action num_ratio_done {
        ragel_num.ratio.num *= ragel_num.sign;
        ragel_type = TYPE_RATIO;
        ragel_cat  = CAT_NUMBER;
    }

    action num_float_frac_init {
        ragel_num.ratio.num = 0;
        ragel_num.ratio.den = 1;
    }
    action num_float_add_frac {
        (ragel_num.ratio.num *= 10) += (fc - '0');
        ragel_num.ratio.den *= 10;
    }
    action num_float_done {
        const auto frac = double(ragel_num.ratio.num) / ragel_num.ratio.den;
        ragel_num.vdouble = ragel_num.vint + ragel_num.sign * frac;
        ragel_type = TYPE_FLOAT;
        ragel_cat  = CAT_NUMBER;
    }

    action num_float_inf {
        ragel_type = TYPE_INF;
        ragel_cat = CAT_NUMBER;
    }

    action num_percent_done {
        ragel_type = TYPE_PERCENT;
        ragel_cat = CAT_NUMBER;
    }

    action num_phase_done {
        ragel_type = TYPE_PHASE;
        ragel_cat = CAT_NUMBER;
    }

    # no actions
    num_sign = '-' | '+';
    num_b = [01]+;
    num_i = '0' | ([1-9] [0-9]**);
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

    num_ratio = ((num_sign @num_sign_done)?
                 num_i $num_ratio_num
                 num_slash
                 num_i $num_ratio_den)
                %num_ratio_done;

    num_frac = '.' @num_float_frac_init ([0-9]+ $num_float_add_frac);
    num_float = (num_int num_frac) %num_float_done;

    num_infinity = (num_sign? num_inf) %num_float_inf;

    num_percent = (
                  (num_float %num_float_done | (num_int %num_int_done) %{ragel_num.vdouble = ragel_num.vint;})
                  '_'?
                  '%')
                  %num_percent_done;
    num_phase =   (
                  (num_float %num_float_done | (num_int %num_int_done) %{ragel_num.vdouble = ragel_num.vint;})
                  ('*' | 'phase' | '_phase')
                  )
                  %num_phase_done;


    number = num_float | num_int | num_bin | num_hex | num_ratio | num_infinity | num_percent | num_phase;
}%%
