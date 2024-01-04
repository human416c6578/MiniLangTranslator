#include <iostream>
#include <cstring>
#include <fstream>
#include <Translator.h>

void displayAST(const ASTNode &node, int depth = 0)
{
    // Display the name and value of the current node
    std::cout << std::string(depth * 2, ' '); // Adjust spacing based on depth for a visual hierarchy
    std::cout << getASTNodeTypeName(node.type) << " | " << static_cast<int>(node.type) << " : " << node.value << std::endl;

    // Recursively display the children
    for (const ASTNode &child : node.children)
    {
        displayAST(child, depth + 1);
    }
}

void displayTokens(std::vector<Token> &tokens)
{
    int size = tokens.size();
    for (int i = 0; i < size; i++)
    {
        std::cout << "Token type: " << tokens.at(i).type << " | Value: \"" << tokens.at(i).value
                  << "\" | Line: " << tokens.at(i).line + 1 << std::endl;
    }

    std::cout << "Number of tokens: " << size << std::endl;
}

int main(int argc, char *argv[])
{
    
    if (argc < 2)
    {
        std::cerr << "Flags: --tokens --ast" << std::endl;
        std::cerr << "Usage: " << argv[0] << " <input_file> <flags>" << std::endl;
        return 1;
    }

    const char *inputFileName = argv[1];
    bool bPrintTokens = false;
    bool bPrintAST = false;

    // Handle flags
    for (int i = 2; i < argc; ++i)
    {
        const char *flag = argv[i];

        if (strcmp(flag, "--tokens") == 0)
        {
            bPrintTokens = true;
        }
        else if (strcmp(flag, "--ast") == 0)
        {
            bPrintAST = true;
        }
        else
        {
            // Handle unrecognized flags
            std::cerr << "Unrecognized flag: " << flag << std::endl;
            return 1;
        }
    }


    // Read script from the specified text file
    std::ifstream file(inputFileName);
    if (!file.is_open())
    {
        std::cerr << "Error opening file: " << inputFileName << std::endl;
        return 1;
    }

    std::string script((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());

    file.close();
    Lexer lexer(script);
    std::vector<Token> tokens = lexer.parse();

    if(bPrintTokens)
        displayTokens(tokens);

    AST ast(tokens);
    ASTNode program({ASTNodeType::PROGRAM});
    ast.parse(program);

    if(bPrintAST)
        displayAST(program);

    std::string c = Translator::ast_to_c(program);

    // Output the generated C string to the terminal
    std::cout << c;

    return 0;
}