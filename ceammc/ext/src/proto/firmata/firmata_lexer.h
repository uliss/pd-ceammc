#ifndef FIRMATA_LEXER_H
#define FIRMATA_LEXER_H

#include <stdint.h>
#include <utility>

namespace firmata {

struct SingleLexerToken {
    int type;
    int value;
};

typedef std::pair<SingleLexerToken, SingleLexerToken> LexerToken;

enum LexerMode {
    LEXER_MODE_NORMAL = 0,
    LEXER_MODE_SYSEX = 1,
    LEXER_MODE_CAPABILITIES = 2,
    LEXER_MODE_SYSEX_RAW = 3
};

LexerToken lexer_token_normal(uint8_t c);
LexerToken lexer_token_sysex(uint8_t c);
LexerToken lexer_token_capabilities(uint8_t c);
LexerToken lexer_token_sysex_raw(uint8_t c);
SingleLexerToken lexer_token_reset();

LexerToken lexer_token(LexerMode mode, uint8_t c);
LexerMode lexer_next_mode(LexerMode mode, const LexerToken& tok);
}

#endif // FIRMATA_LEXER_H
