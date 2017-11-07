#ifndef FIRMATA_PARSER_H
#define FIRMATA_PARSER_H

#include <iostream>

#include "firmata_lexer.h"
#include "firmata_proto.h"

namespace firmata {

class FirmataParser {
    void* parser_state_;
    int status_;
    FirmataMessage msg_;
    LexerMode lexer_mode_;

private:
    FirmataParser(const FirmataParser& fp);
    void operator=(const FirmataParser& fp);
    void pushToken(const SingleLexerToken& tok);

public:
    FirmataParser();
    ~FirmataParser();

    FirmataParser& operator<<(uint8_t byte);

    bool isDone() const;
    bool isError() const;
    bool isSysex() const;
    void reset();
    void clearMessage();

    FirmataMessage& msg() { return msg_; }
    const FirmataMessage& msg() const { return msg_; }

    int value() const;
    std::string str() const;
    int versionMajor() const;
    int versionMinor() const;
};
}

#endif // FIRMATA_PARSER_H
