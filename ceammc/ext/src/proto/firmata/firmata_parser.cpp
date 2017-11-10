#include "firmata_parser.h"
#include "firmata_proto.h"

extern "C" {
#include "firmata_bison.h"
}

using namespace firmata;

FirmataParser::FirmataParser()
    : parser_state_(0)
    , status_(YYPUSH_MORE)
    , lexer_mode_(LEXER_MODE_NORMAL)
{
    parser_state_ = firmata_pstate_new();
    clearMessage();
}

FirmataParser::~FirmataParser()
{
    firmata_pstate_delete(static_cast<firmata_pstate*>(parser_state_));
}

FirmataParser& FirmataParser::operator<<(uint8_t byte)
{
    if (status_ != YYPUSH_MORE || msg_.state != STATE_IN_PROCESS) {
        return *this;
    }

    LexerToken t = lexer_token(lexer_mode_, byte);
    pushToken(t.first);

    // composite tokens
    if (status_ == YYPUSH_MORE && t.second.type > 0)
        pushToken(t.second);

    lexer_mode_ = lexer_next_mode(lexer_mode_, t);

    return *this;
}

void FirmataParser::pushToken(const SingleLexerToken& tok)
{
    FIRMATA_STYPE t;

    if (tok.value < 0)
        t.m = &msg_;
    else
        t.integer = tok.value;

    status_ = firmata_push_parse(static_cast<firmata_pstate*>(parser_state_), tok.type, &t, &msg_);
}

bool FirmataParser::isDone() const
{
    return msg_.state == STATE_DONE;
}

bool FirmataParser::isError() const
{
    return msg_.state == STATE_ERROR;
}

bool FirmataParser::isSysex() const
{
    return msg_.command == PROTO_START_SYSEX;
}

void FirmataParser::reset()
{
    if (msg_.state == STATE_ERROR) {
        pushToken(lexer_token_reset());
    }

    status_ = YYPUSH_MORE;
    lexer_mode_ = LEXER_MODE_NORMAL;
    clearMessage();
}

void FirmataParser::clearMessage()
{
    memset(&msg_, 0, sizeof(msg_));
}

int FirmataParser::value() const
{
    return msg_.value;
}

std::string FirmataParser::str() const
{
    return std::string(msg_.str, msg_.str_len);
}

int FirmataParser::versionMajor() const
{
    return msg_.proto_version.major;
}

int FirmataParser::versionMinor() const
{
    return msg_.proto_version.minor;
}
