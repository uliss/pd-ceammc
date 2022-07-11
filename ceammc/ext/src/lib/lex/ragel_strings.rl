%%{
    machine string_common;

    normal_symbol = ('`' ascii) | [^`"];
    normal_symbol_nz = normal_symbol - 0;

    string_ends = normal_symbol* '"';
}%%
