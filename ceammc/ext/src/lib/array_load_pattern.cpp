#include "array_load_pattern.h"

#include <cassert>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <string>

enum state_t {
    ST_INIT = 0,
    ST_CHAR,
    ST_COMMA,
    ST_RANGE,
    ST_END
};

typedef bool (*transitionFn)(int ch);

static const int SZ = 64;
static std::string current_word;
static std::string vars[SZ];
static size_t var_pos = 0;
static bool in_range = false;

static bool isNumber(const std::string& str)
{
    return str.find_first_not_of("0123456789") == std::string::npos;
}

static bool addChar(int ch)
{
    current_word.append(1, static_cast<char>(ch));
    return true;
}

static bool endRange()
{
    if (var_pos < 1)
        return false;

    if (vars[var_pos - 1].empty() || current_word.empty())
        return false;

    const std::string& prev_word = vars[var_pos - 1];

    if (!isNumber(prev_word)) {
        std::cerr << "[array_pattern] not a number: '" << prev_word << '\'' << std::endl;
        return false;
    }

    if (!isNumber(current_word)) {
        std::cerr << "[array_pattern] not a number: '" << current_word << '\'' << std::endl;
        return false;
    }

    long b = strtol(prev_word.c_str(), 0, 10);
    long e = strtol(current_word.c_str(), 0, 10);

    char buf[64];
    if (b <= e) {
        for (long i = b + 1; i <= e && var_pos < SZ; i++) {
            sprintf(buf, "%ld", i);
            vars[var_pos++].append(buf);
        }
    } else {
        for (long i = b - 1; i >= e && var_pos < SZ; i--) {
            sprintf(buf, "%ld", i);
            vars[var_pos++].append(buf);
        }
    }

    return true;
}

static bool addWord(int)
{
    if (var_pos >= SZ)
        return false;

    if (in_range) {
        if (!endRange())
            return false;
    } else
        vars[var_pos++] = current_word;

    current_word.clear();
    in_range = 0;
    return true;
}

static bool addRange(int)
{
    if (var_pos >= SZ)
        return false;

    if (!isNumber(current_word)) {
        std::cerr << "[array_pattern] not a number: '" << current_word << '\'' << std::endl;
        return false;
    }

    vars[var_pos++] = current_word;
    current_word.clear();
    in_range = true;
    return true;
}

static bool endFn(int)
{
    if (var_pos >= SZ || current_word.empty())
        return false;

    if (in_range) {
        if (!endRange())
            return false;
    } else
        vars[var_pos++] = current_word;

    current_word.clear();
    in_range = false;
    return true;
}

static state_t getState(int c)
{
    if (c == '|')
        return ST_COMMA;
    else if (c == '-')
        return ST_RANGE;
    else
        return ST_CHAR;
}

static void cleanUp()
{
    var_pos = 0;
    in_range = false;
    current_word.clear();

    for (size_t i = 0; i < SZ; i++)
        vars[i].clear();
}

static bool runTransition(transitionFn fn, char ch)
{
    if (fn == 0 || (fn(ch) == false))
        return false;

    return true;
}

static bool parserError(const std::string& str, size_t* count, std::string** vec)
{
    *count = 0;
    *vec = vars;
    std::cerr << "[array_pattern] parse error: '" << str << "'\n";
    return false;
}

bool array_load_parse(const std::string& str, size_t* count, std::string** vec)
{
    // FSM transition table
    // clang-format off
    static transitionFn tab_mov[5][5] = {
        /*            INIT, DIGIT,   COMMA,   RANGE,    END */
        /* INIT  */  { 0,   addChar, 0,       0,        endFn },
        /* CHAR  */  { 0,   addChar, addWord, addRange, endFn },
        /* COMMA */  { 0,   addChar, 0,       0,        0     },
        /* RANGE */  { 0,   addChar, 0,       0,        0     },
        /* END   */  { 0,   0,       0,       0,        0     }
    };
    // clang-format on

    if (str.empty()) {
        *count = 0;
        *vec = vars;
        return true;
    }

    // cleanup
    cleanUp();

    // init
    state_t s = ST_INIT;
    const char* c = str.c_str();
    transitionFn fn = 0;

    // run machine
    while (*c != 0) {
        state_t new_state = getState(*c);
        fn = tab_mov[s][new_state];
        s = new_state;

        if (!runTransition(fn, *c))
            return parserError(str, count, vec);

        c++;
    }

    // stop machine
    fn = tab_mov[s][ST_END];
    if (!runTransition(fn, *c))
        return parserError(str, count, vec);

    *count = var_pos;
    *vec = vars;

    return true;
}

std::vector<std::string> array_pattern_names(const std::string& str)
{
    using namespace std;
    std::vector<std::string> res;
    if (str.empty())
        return res;

    // find open bracket [
    string::size_type open_bracket_pos = str.find_first_of('[', 0);
    if (open_bracket_pos == string::npos)
        return res;

    // find close bracket ]
    string::size_type close_bracket_pos = str.find_first_of(']', open_bracket_pos);
    if (close_bracket_pos == string::npos)
        return res;

    assert(open_bracket_pos < close_bracket_pos);
    assert(close_bracket_pos < str.size());

    std::string prefix = str.substr(0, open_bracket_pos);
    std::string suffix = str.substr(close_bracket_pos + 1, std::string::npos);

    string pattern = str.substr(open_bracket_pos + 1, (close_bracket_pos - 1) - open_bracket_pos);
    if (pattern.empty()) {
        for (size_t i = 1; i <= 8; i++) {
            res.push_back("");
            res.back().append(prefix);
            res.back().append(1, static_cast<char>(48 + i));
            res.back().append(suffix);
        }
    } else {
        size_t pat_count = 0;
        std::string* patterns = 0;

        if (!array_load_parse(pattern, &pat_count, &patterns))
            return res;

        for (size_t i = 0; i < pat_count; i++) {
            std::string p(prefix);
            p.append(patterns[i]);
            p.append(suffix);
            res.push_back(p);
        }
    }

    return res;
}
