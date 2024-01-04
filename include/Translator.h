#include <AST.h>

class Translator
{
public:
    static std::string ast_to_c(ASTNode node, int depth = 0)
    {
        auto add_indentation = [depth]() {
            return std::string(4 * depth, ' ');
        };

        switch (node.type)
        {
        case ASTNodeType::PROGRAM:
        {
            depth++;
            std::string program_code;
            program_code += "#include <iostream>\n\nint main()\n{\n";
            for (const auto &child : node.children)
            {
                program_code += ast_to_c(child, depth) + "\n";
            }
            program_code += "\n}";
            return program_code;
        }
        case ASTNodeType::VARIABLE_DECLARATION:
        {
            std::string identifier = ast_to_c(node.children[0], depth);
            std::string value = "";
            
            if(node.children.size() > 1) 
            {
                value = " = " + ast_to_c(node.children[1], depth);
            }
            return add_indentation() + "int " + identifier + value + ";";
        }
        case ASTNodeType::ASSIGNMENT:
        {
            std::string left_operand = ast_to_c(node.children[0], depth);
            std::string right_operand = ast_to_c(node.children[1], depth);
            return add_indentation() + left_operand + " = " + right_operand +";";
        }
        case ASTNodeType::WHILE_LOOP:
        {
            std::string condition = ast_to_c(node.children[0], depth);
            std::string block = ast_to_c(node.children[1], depth + 1);
            return add_indentation() + "while (" + condition + ")\n" + add_indentation() + "{\n" + block + add_indentation() + "}\n";
        }
        case ASTNodeType::IF_STATEMENT:
        {
            std::string condition = ast_to_c(node.children[0], depth);
            std::string block = ast_to_c(node.children[1], depth + 1);
            std::string value = add_indentation() + "if (" + condition + ") \n" + add_indentation() + "{\n" + block + add_indentation() + "}\n";
            if(node.children.size() > 2) // has else 
            {
                value += add_indentation() + "else\n" + add_indentation() + "{\n" + ast_to_c(node.children[2], depth + 1) + add_indentation() + "}\n";
            }
            return value;
        }
        case ASTNodeType::PARENTHESES:
        {
            return "(" + ast_to_c(node.children[0], depth) + ")";
        }
        case ASTNodeType::PLUS_OPERATION:
        {
            std::string left_operand = ast_to_c(node.children[0], depth);
            std::string right_operand = ast_to_c(node.children[1], depth);
            return left_operand + " + " + right_operand;
        }
        case ASTNodeType::MINUS_OPERATION:
        {
            std::string left_operand = ast_to_c(node.children[0], depth);
            std::string right_operand = ast_to_c(node.children[1], depth);
            return left_operand + " - " + right_operand;
        }
        case ASTNodeType::MULTIPLY_OPERATION:
        {
            std::string left_operand = ast_to_c(node.children[0], depth);
            std::string right_operand = ast_to_c(node.children[1], depth);
            return left_operand + " * " + right_operand;
        }
        case ASTNodeType::DIVIDE_OPERATION:
        {
            std::string left_operand = ast_to_c(node.children[0], depth);
            std::string right_operand = ast_to_c(node.children[1], depth);
            return left_operand + " / " + right_operand;
        }
        case ASTNodeType::GREATER_OPERATION:
        {
            std::string left_operand = ast_to_c(node.children[0], depth);
            std::string right_operand = ast_to_c(node.children[1], depth);
            return left_operand + " > " + right_operand;
        }
        case ASTNodeType::LESS_OPERATION:
        {
            std::string left_operand = ast_to_c(node.children[0], depth);
            std::string right_operand = ast_to_c(node.children[1], depth);
            return left_operand + " < " + right_operand;
        }
        case ASTNodeType::COMPARE_OPERATION:
        {
            std::string left_operand = ast_to_c(node.children[0], depth);
            std::string right_operand = ast_to_c(node.children[1], depth);
            return left_operand + " == " + right_operand;
        }
        case ASTNodeType::AND_OPERATION:
        {
            std::string left_operand = ast_to_c(node.children[0], depth);
            std::string right_operand = ast_to_c(node.children[1], depth);
            return left_operand + " && " + right_operand;
        }
        case ASTNodeType::BIT_AND_OPERATION:
        {
            std::string left_operand = ast_to_c(node.children[0], depth);
            std::string right_operand = ast_to_c(node.children[1], depth);
            return left_operand + " & " + right_operand;
        }
        case ASTNodeType::OR_OPERATION:
        {
            std::string left_operand = ast_to_c(node.children[0], depth);
            std::string right_operand = ast_to_c(node.children[1], depth);
            return left_operand + " || " + right_operand;
        }
        case ASTNodeType::BIT_OR_OPERATION:
        {
            std::string left_operand = ast_to_c(node.children[0], depth);
            std::string right_operand = ast_to_c(node.children[1], depth);
            return left_operand + " | " + right_operand;
        }
        case ASTNodeType::BLOCK:
        {
            std::string block_code;
            for (const auto &child : node.children)
            {
                block_code += ast_to_c(child, depth) + "\n";
            }
            return block_code;
        }
        case ASTNodeType::IDENTIFIER:
        {
            std::string array_specifier = "";
            if (node.children.size() > 0)
            {
                array_specifier = ast_to_c(node.children[0], depth);
                
            }
            return node.value + array_specifier;
        }
        case ASTNodeType::ARRAY_SPECIFIER:
        {
            return "[" + ast_to_c(node.children[0], depth) + "]";
        }
        case ASTNodeType::NUMBER_LITERAL:
        {
            return node.value;
        }
        case ASTNodeType::STRING_LITERAL:
        {
            return "\"" + node.value + "\"";
        }
        case ASTNodeType::PRINT_STATEMENT:
        {
            return add_indentation() + "std::cout << " + ast_to_c(node.children[0], depth) + ";";
        }
        case ASTNodeType::READ_STATEMENT:
        {
            return add_indentation() + "std::cin >> " + ast_to_c(node.children[0], depth) + ";";
        }
        
        default:
        {
            // Handle other node types as needed
            return "";
        }
        }
    }
};