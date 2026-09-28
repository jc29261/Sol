#pragma once

#include "Token.h"

#include <string>
#include <vector>

namespace sol {
    class Lexer {
        public:
            explicit Lexer(std::string source);

            std::vector<Token> tokenize();

        private:
            char peek(std::size_t offset = 0) const;
            char advance();

            bool atEnd() const;

            void skipWhitespace();
            bool skipComment();

            Token identifier();
            Token typedLiteral();

            Token makeToken(
                TokenKind kind,
                std::size_t start,
                std::size_t startLine,
                std::size_t startColumn
            );

            Token simpleToken(TokenKind kind);

            TokenKind keywordKind(const std::string& word) const;

        private:
            std::string source;

            std::size_t position = 0;

            std::size_t line = 1;
            std::size_t column = 1;
    };
}