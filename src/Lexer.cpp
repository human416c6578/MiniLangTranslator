#include "Lexer.h"

const char *getTokenTypeName(TokenType type)
{
    switch (type)
    {
    case TokenType::VAR:
        return "VAR";
    case TokenType::NAME:
        return "NAME";
    case TokenType::NUM:
        return "NUM";
    case TokenType::IF:
        return "IF";
    case TokenType::WHILE:
        return "WHILE";
    case TokenType::PRINT:
        return "PRINT";
    case TokenType::READ:
        return "READ";
    case TokenType::END:
        return "END";
    case TokenType::EQUALS:
        return "EQUALS";
    case TokenType::PLUS:
        return "PLUS";
    case TokenType::MINUS:
        return "MINUS";
    case TokenType::MULTIPLY:
        return "MULTIPLY";
    case TokenType::DIVIDE:
        return "DIVIDE";
    case TokenType::LESS_THAN:
        return "LESS_THAN";
    case TokenType::GREATER_THAN:
        return "GREATER_THAN";
    case TokenType::OPEN_BRACKET:
        return "OPEN_BRACKET";
    case TokenType::CLOSE_BRACKET:
        return "CLOSE_BRACKET";
    case TokenType::OPEN_SQUARE_BRACKET:
        return "OPEN_SQUARE_BRACKET";
    case TokenType::CLOSE_SQUARE_BRACKET:
        return "CLOSE_SQUARE_BRACKET";
    case TokenType::SEMICOLON:
        return "SEMICOLON";
    case TokenType::COMMA:
        return "COMMA";
    case TokenType::STRING:
        return "STRING";
    case TokenType::NEWLINE:
        return "NEWLINE";
    case TokenType::EOF_TOKEN:
        return "EOF_TOKEN";
    default:
        return "UNKNOWN";
    }
}

Lexer::Lexer(const std::string &script) : script_(script), pos_(0) {}

std::vector<Token> Lexer::parse()
{
    std::vector<Token> tokens;
    int line = 0;
    while (pos_ < script_.size())
    {
        char currentChar = script_[pos_];
        if (currentChar == '\n')
            line++;
        if (currentChar == '#')
        {
            // Skip the entire line if it starts with '#'
            skipLine();
        }
        else if (currentChar == '"')
        {
            tokens.push_back(parseString(line));
        }
        else if (isspace(currentChar))
        {
            // Ignore whitespaces
            pos_++;
        }
        else if (isdigit(currentChar))
        {
            tokens.push_back(parseNumber(line));
        }
        else if (isalpha(currentChar))
        {
            tokens.push_back(parseKeywordOrIdentifier(line));
        }
        else
        {
            // Handle other characters
            switch (currentChar)
            {
            // Handle symbols
            case '+':
                tokens.push_back({PLUS, "+", line});
                break;
            case '-':
                tokens.push_back({MINUS, "-", line});
                break;
            case '*':
                tokens.push_back({MULTIPLY, "*", line});
                break;
            case '/':
                tokens.push_back({DIVIDE, "/", line});
                break;
            case '<':
                tokens.push_back({LESS_THAN, "<", line});
                break;
            case '>':
                tokens.push_back({GREATER_THAN, ">", line});
                break;
            case ',':
                tokens.push_back({COMMA, ",", line});
                break;
            case '(':
                tokens.push_back({OPEN_BRACKET, "(", line});
                break;
            case ')':
                tokens.push_back({CLOSE_BRACKET, ")", line});
                break;
            case '[':
                tokens.push_back({OPEN_SQUARE_BRACKET, "[", line});
                break;
            case ']':
                tokens.push_back({CLOSE_SQUARE_BRACKET, "]", line});
                break;
            case '&':
                tokens.push_back(parseSymbols(line));
                break;
            case '|':
                tokens.push_back(parseSymbols(line));
                break;
            case '=':
                tokens.push_back(parseSymbols(line));
                break;
            default:
                std::cerr << "Unexpected character: " << currentChar << std::endl;
                break;
            }
            pos_++;
        }
    }

    // Add an EOF token to mark the end of the token stream
    tokens.push_back({EOF_TOKEN, ""});

    return tokens;
}

Token Lexer::parseString(int line)
{
    std::string value;
    pos_++; // Skip the opening quote
    while (pos_ < script_.size() && script_[pos_] != '"')
    {
        value += script_[pos_++];
    }
    if (pos_ < script_.size() && script_[pos_] == '"')
    {
        // Skip the closing quote
        pos_++;
    }
    return {STRING, value, line};
}

Token Lexer::parseNumber(int line)
{
    std::string value;
    while (isdigit(script_[pos_]))
    {
        value += script_[pos_++];
    }
    return {NUM, value, line};
}

Token Lexer::parseKeywordOrIdentifier(int line)
{
    std::string value;
    while (isalnum(script_[pos_]))
    {
        value += script_[pos_++];
    }

    // Check if the identifier is a keyword
    if (value == "var")
        return {VAR, value, line};
    else if (value == "if")
        return {IF, value, line};
    else if (value == "while")
        return {WHILE, value, line};
    else if (value == "print")
        return {PRINT, value, line};
    else if (value == "read")
        return {READ, value, line};
    else if (value == "else")
        return {ELSE, value, line};
    else if (value == "end")
        return {END, value, line};
    else
        return {NAME, value, line}; // Assume it's an identifier if not a keyword
}

Token Lexer::parseSymbols(int line)
{
    switch (script_[pos_])
    {
    case '&':
        if(script_[pos_ + 1] == '&') {
            pos_++;
            return {AND, "&&", line};
        }
        else
            return {BIT_AND, "&", line};
    case '|':
        if(script_[pos_ + 1] == '|') {
            pos_++;
            return {OR, "||", line};
        }
        else
            return {BIT_OR, "|", line};
    case '=':
        if(script_[pos_ + 1] == '='){
            pos_++;
            return {COMPARE_EQUALS, "==", line};
        }
        else
            return {EQUALS, "=", line};
    }

    return {};
}

void Lexer::skipLine()
{
    while (pos_ < script_.size() && script_[pos_] != '\n')
    {
        pos_++;
    }
}
