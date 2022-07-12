%%{
    machine string_common;

    normal_symbol = ('`' ascii) | [^`"];
    normal_symbol_nz = normal_symbol - 0;

    string_ends = normal_symbol* '"';

    str_escape    = '`';
    str_squote    = "'";
    str_dquote    = '"';

    esc_escape    = str_escape str_escape;
    esc_space     = str_escape ' ';
    esc_squote    = str_escape "'";
    esc_dquote    = str_escape '"';
    esc_comma     = str_escape '.';
    esc_semicolon = str_escape ':';
    esc_slash     = str_escape '/';
    str_envvar    = '%' [A-Z_0-9]{1,16} '%';

}%%
