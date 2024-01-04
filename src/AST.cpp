#include <AST.h>

const char *getASTNodeTypeName(ASTNodeType type)
{
    switch (type)
    {
    case ASTNodeType::PROGRAM:
        return "PROGRAM";
    case ASTNodeType::VARIABLE_DECLARATION:
        return "VARIABLE_DECLARATION";
    case ASTNodeType::ARRAY_SPECIFIER:
        return "ARRAY_SPECIFIER";
    case ASTNodeType::ASSIGNMENT:
        return "ASSIGNMENT";
    case ASTNodeType::BLOCK:
        return "BLOCK";
    case ASTNodeType::IF_STATEMENT:
        return "IF_STATEMENT";
    case ASTNodeType::WHILE_LOOP:
        return "WHILE_LOOP";
    case ASTNodeType::PRINT_STATEMENT:
        return "PRINT_STATEMENT";
    case ASTNodeType::READ_STATEMENT:
        return "READ_STATEMENT";
    case ASTNodeType::PLUS_OPERATION:
        return "PLUS_OPERATION";
    case ASTNodeType::MINUS_OPERATION:
        return "MINUS_OPERATION";
    case ASTNodeType::MULTIPLY_OPERATION:
        return "MULTIPLY_OPERATION";
    case ASTNodeType::DIVIDE_OPERATION:
        return "DIVIDE_OPERATION";
    case ASTNodeType::GREATER_OPERATION:
        return "GREATER_OPERATION";
    case ASTNodeType::LESS_OPERATION:
        return "LESS_OPERATION";
    case ASTNodeType::COMPARE_OPERATION:
        return "COMPARE_OPERATION";
    case ASTNodeType::AND_OPERATION:
        return "AND_OPERATION";
    case ASTNodeType::BIT_AND_OPERATION:
        return "BIT_AND_OPERATION";
    case ASTNodeType::OR_OPERATION:
        return "OR_OPERATION";
    case ASTNodeType::BIT_OR_OPERATION:
        return "BIT_OR_OPERATION";
    case ASTNodeType::IDENTIFIER:
        return "IDENTIFIER";
    case ASTNodeType::NUMBER_LITERAL:
        return "NUMBER_LITERAL";
    case ASTNodeType::STRING_LITERAL:
        return "STRING_LITERAL";
    case ASTNodeType::PARENTHESES:
        return "PARENTHESES";
    default:
        return "UNKNOWN";
    }
}

void AST::parse(ASTNode &Program)
{
    while (pos_ < tokens_.size())
    {
        switch (current().type)
        {
        case TokenType::VAR:
            Program.children.push_back(parseDeclaration());
            break;
        case TokenType::NAME:
            Program.children.push_back(parseAssignment());
            break;
        case TokenType::PRINT:
            Program.children.push_back(parsePrint());
            break;
        case TokenType::READ:
            Program.children.push_back(parseRead());
            break;
        case TokenType::WHILE:
            Program.children.push_back(parseWhile());
            break;
        case TokenType::IF:
            Program.children.push_back(parseIf());
            break;
        default:
            consume();
            break;
        }
    }
}

ASTNode AST::parseArraySpecifier()
{
    ASTNode arraySpecifierNode;
    arraySpecifierNode.type = ASTNodeType::ARRAY_SPECIFIER;

    consume(); // consume open bracket
    // Parse the expression inside the array specifier
    arraySpecifierNode.children.push_back(parseExpression());

    if (current().type != TokenType::CLOSE_SQUARE_BRACKET)
    {
        std::cerr << "[parseArraySpecifier] Expected CLOSE_SQUARE_BRACKET, but got " << getTokenTypeName(current().type) << std::endl;
    }
    consume(); // consume close square bracket
    return arraySpecifierNode;
}

ASTNode AST::parseIdentifier()
{
    ASTNode identifierNode;
    identifierNode.type = ASTNodeType::IDENTIFIER;
    identifierNode.value = current().value;
    consume(); // consume name

    // Check for an array specifier
    if (current().type == TokenType::OPEN_SQUARE_BRACKET)
    {
        identifierNode.children.push_back(parseArraySpecifier());
    }

    return identifierNode;
}

ASTNode AST::parseExpression()
{
    if (current().type == TokenType::NUM || current().type == TokenType::NAME || current().type == TokenType::OPEN_BRACKET)
    {
        ASTNode numNode;

        if (current().type == TokenType::NUM)
        {
            numNode.type = ASTNodeType::NUMBER_LITERAL;
            numNode.value = current().value;
            consume(); // consume number
        }
        else if (current().type == TokenType::NAME)
        {
            numNode = parseIdentifier();
            // parseIdentifier already returns the next token
        }
        else if (current().type == TokenType::OPEN_BRACKET)
        {
            consume(); // consume '('
            ASTNode Parentheses;
            Parentheses.type = ASTNodeType::PARENTHESES;
            Parentheses.children.push_back(parseExpression());
            consume(); // consume ')'

            numNode = Parentheses;
        }

        if (current().type == TokenType::PLUS ||
            current().type == TokenType::MINUS ||
            current().type == TokenType::MULTIPLY ||
            current().type == TokenType::DIVIDE ||
            current().type == TokenType::GREATER_THAN ||
            current().type == TokenType::LESS_THAN ||
            current().type == TokenType::COMPARE_EQUALS ||
            current().type == TokenType::AND ||
            current().type == TokenType::BIT_AND ||
            current().type == TokenType::OR ||
            current().type == TokenType::BIT_OR)
        {

            ASTNode exprNode;

            switch (current().type)
            {
            case TokenType::PLUS:
                exprNode.type = ASTNodeType::PLUS_OPERATION;
                break;
            case TokenType::MINUS:
                exprNode.type = ASTNodeType::MINUS_OPERATION;
                break;
            case TokenType::MULTIPLY:
                exprNode.type = ASTNodeType::MULTIPLY_OPERATION;
                break;
            case TokenType::DIVIDE:
                exprNode.type = ASTNodeType::DIVIDE_OPERATION;
                break;
            case TokenType::GREATER_THAN:
                exprNode.type = ASTNodeType::GREATER_OPERATION;
                break;
            case TokenType::LESS_THAN:
                exprNode.type = ASTNodeType::LESS_OPERATION;
                break;
            case TokenType::COMPARE_EQUALS:
                exprNode.type = ASTNodeType::COMPARE_OPERATION;
                break;
            case TokenType::AND:
                exprNode.type = ASTNodeType::AND_OPERATION;
                break;
            case TokenType::BIT_AND:
                exprNode.type = ASTNodeType::BIT_AND_OPERATION;
                break;
            case TokenType::OR:
                exprNode.type = ASTNodeType::OR_OPERATION;
                break;
            case TokenType::BIT_OR:
                exprNode.type = ASTNodeType::BIT_OR_OPERATION;
                break;
            }
            // consume operator
            consume();
            exprNode.children.push_back(numNode);
            exprNode.children.push_back(parseExpression());
            return exprNode;
        }
        else
        {
            return numNode;
        }
    }
    else
    {
        std::cerr << "Error, wrong operand!";
    }

    return {};
}

ASTNode AST::parseAssignment()
{
    ASTNode assignNode;
    assignNode.type = ASTNodeType::ASSIGNMENT;
    assignNode.children.push_back(parseIdentifier());

    if (current().type == TokenType::EQUALS)
    {
        consume();
        assignNode.children.push_back(parseExpression());
    }
    else
    {
        std::cerr << "Error: Bad Token" << current().line;
    }

    return assignNode;
}

ASTNode AST::parseDeclaration()
{
    ASTNode varNode;
    varNode.type = ASTNodeType::VARIABLE_DECLARATION;
    consume(); // Move past the 'var' token

    if (current().type == TokenType::NAME)
    {
        // Parse the variable name
        varNode.children.push_back(parseIdentifier());
        // Check for assignment specifier
        if (current().type == TokenType::EQUALS)
        {
            consume(); // Move past the 'equals' token
            varNode.children.push_back(parseExpression());
        }
    }
    else
    {
        std::cerr << "Error, invalid token!";
    }

    return varNode;
}

ASTNode AST::parsePrint()
{
    ASTNode printNode;
    printNode.type = ASTNodeType::PRINT_STATEMENT;
    if (peek().type == TokenType::STRING || peek().type == TokenType::NAME)
    {
        consume(); // Move past the 'print' token
        ASTNode val;
        if (current().type == TokenType::NAME)
        {
            val = parseIdentifier();
            // parseIdentifier already leave us at the next token
        }
        else
        {
            val.type = ASTNodeType::STRING_LITERAL;
            val.value = current().value;
            consume(); // Move past the 'string' token
        }

        printNode.children.push_back(val);
    }
    else
    {
        std::cerr << "Error: Wrong token after print!";
    }

    return printNode;
}

ASTNode AST::parseRead()
{
    ASTNode readNode;
    readNode.type = ASTNodeType::READ_STATEMENT;
    consume();
    if (current().type == TokenType::NAME)
    {
        readNode.children.push_back(parseIdentifier());
        // parseIdentifier already leave us at the next token
    }
    else
    {
        std::cerr << "Error: Wrong token after read!";
    }

    return readNode;
}

ASTNode AST::parseBlock()
{
    ASTNode node;
    node.type = ASTNodeType::BLOCK;

    while (current().type != TokenType::END && current().type != TokenType::ELSE)
    {
        switch (current().type)
        {
        case TokenType::VAR:
            node.children.push_back(parseDeclaration());
            break;
        case TokenType::NAME:
            node.children.push_back(parseAssignment());
            break;
        case TokenType::PRINT:
            node.children.push_back(parsePrint());
            break;
        case TokenType::READ:
            node.children.push_back(parseRead());
            break;
        case TokenType::WHILE:
            node.children.push_back(parseWhile());
            break;
        case TokenType::IF:
            node.children.push_back(parseIf());
            break;
        default:
            consume();
            break;
        }
    }

    return node;
}

ASTNode AST::parseWhile()
{
    ASTNode whileNode;
    whileNode.type = ASTNodeType::WHILE_LOOP;
    consume(); // consume while
    whileNode.children.push_back(parseExpression());
    whileNode.children.push_back(parseBlock());
    consume(); // consume end
    return whileNode;
}

ASTNode AST::parseIf()
{
    ASTNode ifNode;
    ifNode.type = ASTNodeType::IF_STATEMENT;
    consume(); // skip if
    ifNode.children.push_back(parseExpression());
    ifNode.children.push_back(parseBlock());
    if (current().type == TokenType::ELSE)
    {
        consume(); // consume else
        ifNode.children.push_back(parseBlock());
    }
    else
    {
        consume(); // consume end
    }

    return ifNode;
}

Token AST::current()
{
    return tokens_[pos_];
}

Token AST::peek()
{
    return tokens_[pos_ + 1];
}

void AST::consume()
{
    pos_++;
}