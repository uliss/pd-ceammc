%%{
    machine string_common;

    normal_symbol = ('`' ascii) | [^`"];
    normal_symbol_nz = normal_symbol - 0;

    string_ends = normal_symbol* '"';

    str_escape    = '`';
    str_squote    = "'";
    str_dquote    = '"';
    str_space     = ' ';
    str_comma     = ',';
    str_semicolon = ';';
    str_backslash = '\\';
    str_lpar      = '(';
    str_rpar      = ')';
    str_lbrac     = '[';
    str_rbrac     = ']';
    str_colon     = ':';
    str_at        = '@';
    str_perc      = '%';
    str_hash      = '#';

    str_envvar    = '%' [A-Z_0-9]{1,16} '%';

    esc_escape    = str_escape str_escape;
    esc_space     = str_escape str_space;
    esc_dquote    = str_escape str_dquote;
    esc_comma     = str_escape '.';
    esc_semicolon = str_escape ':';
    esc_slash     = str_escape '/';
    esc_at        = str_escape '@';

    all_escapes =
        str_escape |
        str_dquote |
        str_space |
        str_comma |
        str_semicolon |
        str_lpar |
        str_rpar |
        str_lbrac |
        str_rbrac |
        str_colon |
        str_at |
        str_perc |
        str_hash |
        str_backslash;
}%%
