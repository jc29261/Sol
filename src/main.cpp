#include "sol/Lexer.h"
#include "sol/Token.h"

#include <fstream>
#include <iostream>
#include <sstream>

int main(int argc, char* argv[]) {
    if (argc != 2) {
        std::cerr << "Usage: solc <file.sol>\n";
        return 1;
    }

    std::ifstream file(argv[1]);

    if (!file) {
        std::cerr << "Could not open: "
                  << argv[1] << '\n';
        return 1;
    }

    std::stringstream buffer;
    buffer << file.rdbuf();

    try {
        sol::Lexer lexer(buffer.str());

        auto tokens = lexer.tokenize();

        for (const auto& token : tokens) {
            std::cout
                << token.line
                << ':'
                << token.column
                << " "
                << sol::tokenKindName(token.kind)
                << "  \""
                << token.lexeme
                << "\"\n";
        }
    }
    catch (const std::exception& error) {
        std::cerr << "Lexer error: "
                  << error.what()
                  << '\n';

        return 1;
    }

    return 0;
}