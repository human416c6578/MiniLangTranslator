#pragma once
#include <Lexer.h>

enum class ASTNodeType
{
    PROGRAM,
    VARIABLE_DECLARATION,
    ARRAY_SPECIFIER,
    ASSIGNMENT,
    BLOCK,
    IF_STATEMENT,
    WHILE_LOOP,
    PRINT_STATEMENT,
    READ_STATEMENT,
    PLUS_OPERATION,
    MINUS_OPERATION,
    MULTIPLY_OPERATION,
    DIVIDE_OPERATION,
    GREATER_OPERATION,
    LESS_OPERATION,
    COMPARE_OPERATION,
    AND_OPERATION,
    BIT_AND_OPERATION,
    OR_OPERATION,
    BIT_OR_OPERATION,
    IDENTIFIER,
    NUMBER_LITERAL,
    STRING_LITERAL,
    PARENTHESES,
};

const char *getASTNodeTypeName(ASTNodeType type);

struct ASTNode
{
    ASTNodeType type;
    std::string value; // For variables, numbers, strings, etc.
    std::vector<ASTNode> children;

    ASTNode() {}
    ASTNode(ASTNodeType type, const std::string &value = "") : type(type), value(value) {}
};

class AST
{
public:
    AST(const std::vector<Token> &tokens): tokens_(tokens), pos_(0) {}

    void parse(ASTNode &Program);

private:
    ASTNode parseArraySpecifier();

    ASTNode parseIdentifier();

    ASTNode parseExpression();

    ASTNode parseAssignment();

    ASTNode parseDeclaration();

    ASTNode parsePrint();

    ASTNode parseRead();

    ASTNode parseBlock();

    ASTNode parseWhile();

    ASTNode parseIf();

private:
    const std::vector<Token> &tokens_;
    std::size_t pos_;

    Token current();

    Token peek();

    void consume();
};
