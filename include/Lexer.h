#pragma once
#include <iostream>
#include <vector>
#include <sstream>

enum TokenType
{
    VAR,
    NAME,
    NUM,
    IF,
    WHILE,
    PRINT,
    READ,
    ELSE,
    END,
    EQUALS,
    PLUS,
    MINUS,
    MULTIPLY,
    DIVIDE,
    LESS_THAN,
    GREATER_THAN,
    COMPARE_EQUALS,
    AND,
    OR,
    BIT_AND,
    BIT_OR,
    OPEN_BRACKET,
    CLOSE_BRACKET,
    OPEN_SQUARE_BRACKET,
    CLOSE_SQUARE_BRACKET,
    SEMICOLON,
    COMMA,
    STRING,
    NEWLINE,
    EOF_TOKEN
};

struct Token
{
    TokenType type;
    std::string value;
    int line;
};

const char *getTokenTypeName(TokenType type);

class Lexer
{
public:
    Lexer(const std::string &script);

    std::vector<Token> parse();

private:
    Token parseString(int line);

    Token parseNumber(int line);

    Token parseKeywordOrIdentifier(int line);

    Token parseSymbols(int line);

    void skipLine();

private:
    const std::string &script_;
    std::size_t pos_;
};
