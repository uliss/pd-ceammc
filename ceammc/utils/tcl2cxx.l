%top {
    # include <string>
    # include <iostream>
    constexpr int LINEWD = 1000;
%}

%class {
    int nl {0};
    bool first {true};

    void out_begin() { std::cout << "\tsys_vgui(\"%s\", \""; }
    void out_end()   { std::cout << "\");\n"; }
%}

%option freespace unicode
%option lexer=tcl2cxx_lexer
%option header-file=tcl2cxx_lexer.h
%option outfile=tcl2cxx_lexer.cpp
/*%option debug*/
%option noyywrap
%option reentrant
%option fast

space    [ \t]+
nl       \r?\n
comment  ^{space}?#.*{nl}
emptyln  ^{space}?{nl}

%%

{emptyln} { }
{comment} { }
\r/\n     { }
\\        { std::cout << "\\\\"; nl++; }
\"        { std::cout << "\\\""; nl++; }
\n        { std::cout << "\\n"; nl++; }
{space}   {
            if (columno() != 0) {
                std::cout << ' ';
                nl++;
            }
          }
.         {
            if (first) {
                out_begin();
                first = false;
            }

            bool is_nl = nl++ > LINEWD;
            if (is_nl) {
                out_end();
                out_begin();
                nl = 0;
            }

            std::cout << text();
          }
<<EOF>>   { out_end(); return -1; }
%%


