%top {
    # include <string>
    # include <array>
    # include <stdexcept>

    # include "lex/select.parser.hpp"

    using token = ceammc::SelectParser::token;
%}

%class {
    public:  
        enum MatchType {
            MATCH_EQUAL,
            MATCH_EPSILON,
            MATCH_RANGE_OO,
            MATCH_RANGE_CC,
            MATCH_RANGE_OC,
            MATCH_RANGE_CO,
            MATCH_SET,
            MATCH_LESS,
            MATCH_LESS_EQ,
            MATCH_GREATER,
            MATCH_GREATER_EQ
        };

        struct LexerAtom {
            std::string txt;
            bool is_float;
        };

        struct MatchData {
            ceammc::Atom atoms[16];
            size_t natoms = { 0 };
            MatchType type = { MATCH_EQUAL };

            MatchData(){}

            MatchData(double v) : natoms(1), type(MATCH_EQUAL) {
                atoms[0].setFloat(v, true);
            }

            MatchData(const std::string& s) : natoms(1), type(MATCH_EQUAL) {
                atoms[0].setSymbol(gensym(s.c_str()), true);
            }

            static MatchData epsilon(double a, double b) {
                MatchData res;
                res.natoms = 2;
                res.type = MATCH_EPSILON;
                res.atoms[0].setFloat(a, true);
                res.atoms[1].setFloat(b, true);
                return res;
            }

            static MatchData range(double a, double b, MatchType t) {
                MatchData res;
                res.natoms = 2;
                res.type = t;
                res.atoms[0].setFloat(a, true);
                res.atoms[1].setFloat(b, true);
                return res;
            }

            static MatchData compare(double x, MatchType t) {
                MatchData res;
                res.natoms = 1;
                res.type = t;
                res.atoms[0].setFloat(x, true);
                return res;
            }
        };

        void pushMatch(const MatchData& d)
        {
            if(nmatches_ < matches_.size())
                matches_[nmatches_++] = d;
        }

        void pushLexerAtom(const LexerAtom& a)
        {
            atoms_.push_back(a);
        }

        void popLexerAtoms()
        {
            if(atoms_.size() == 1 && atoms_[0].is_float)
                pushMatch(MatchData(std::strtod(atoms_[0].txt.c_str(), nullptr)));
            else {
                std::string res;
                for(auto& a: atoms_)
                    res += a.txt;

                pushMatch(MatchData(res));
            }

            atoms_.clear();
        }

        const MatchData& operator[](size_t idx) const {
            if(idx >= nmatches_)
                throw std::out_of_range("invalid index");

            return matches_[idx];
        }

        size_t numMatches() const { return nmatches_; }
        void clearMatches() { nmatches_ = 0; }
        void mergeMatch() {
            if(nmatches_ < 2)
                return;

            auto& m0 = matches_[nmatches_-2];
            const auto& m1 = matches_[nmatches_-1];
            for(size_t i = 0; i < m1.natoms; i++)
                m0.atoms[m0.natoms++] = m1.atoms[i];


            m0.type = MATCH_SET;
            nmatches_ -= 1;
        }

        void setErrorMsg(const std::string& str) { error_msg_ = str; }

    private:
        std::array<MatchData, 64> matches_;
        std::vector<LexerAtom> atoms_;
        size_t nmatches_ = {0};
        std::string error_msg_;
%}

%option freespace unicode
%option namespace=ceammc
%option lexer=SelectLexer
%option header-file=select.lexer.h
%option outfile=select.lexer.cpp
/*%option graphs-file*/
/*%option debug*/
/*%option perf-report*/
%option bison-cc-namespace="ceammc"
%option bison-cc-parser="SelectParser"
%option bison-cc
%option bison-complete
%option noyywrap
%option reentrant
%option noindent
/*%option fast*/

space       [ \t\n]+

udec        0 | [1-9] [0-9]*
frac        \. \d+

int         [-+]? {udec}
uint        {udec}
float       {int} {frac}?
ufloat      {udec} {frac}?

range       \.\.
range_clb   \[
range_crb   \]
range_olb   \(
range_orb   \)
epsilon     ~
or          \|
gt          >
ge          >=
lt          <
le          <=
not         !
symbol      [^ \t\n|\d]+

%xstate RANGE
%xstate EPSILON

%%

{space}                 { return SelectParser::make_SPACE(); }
{epsilon}/{ufloat}      { return SelectParser::make_EPSILON(); }
{range}/{float}         { return SelectParser::make_RANGE(); }
{range_clb}/{float}     { return SelectParser::make_OPEN_BRACKET('['); }
{range_crb}             { return SelectParser::make_CLOSE_BRACKET(']'); }
{range_olb}/{float}     { return SelectParser::make_OPEN_PAR('('); }
{range_orb}             { return SelectParser::make_CLOSE_PAR(')'); }
{or}                    { return SelectParser::make_OR(); }
{ge}/{float}            { return SelectParser::make_OP_GREATER_EQ(); }
{gt}/{float}            { return SelectParser::make_OP_GREATER(); }
{lt}/{float}            { return SelectParser::make_OP_LESS(); }
{le}/{float}            { return SelectParser::make_OP_LESS_EQ(); }
{float}                 { return SelectParser::make_FLOAT(text()); }
/*{float}/{symbol}      { return SelectParser::make_FLOAT(text()); }*/
{symbol}                { return SelectParser::make_SYMBOL(text()); }

<*> .                   {   }

%%

