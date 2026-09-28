#include "sol/Lexer.h"

#include <cctype>
#include <stdexcept>
#include <unordered_map>

namespace sol {

    Lexer::Lexer(std::string source)
        : source(std::move(source)) {}

    char Lexer::peek(std::size_t offset) const {
        if (position + offset >= source.size())
            return '\0';

        return source[position + offset];
    }

    char Lexer::advance() {
        if (atEnd())
            return '\0';

        char c = source[position++];

        if (c == '\n') {
            ++line;
            column = 1;
        } else {
            ++column;
        }

        return c;
    }

    bool Lexer::atEnd() const {
        return position >= source.size();
    }

    void Lexer::skipWhitespace() {
        while (!atEnd()) {
            char c = peek();

            if (c == ' ' || c == '\t' || c == '\r') {
                advance();
                continue;
            }

            break;
        }
    }

    bool Lexer::skipComment() {
        if (peek() != '<' || peek(1) != '?' || peek(2) != 's')
            return false;

        advance();
        advance();
        advance();

        while (!atEnd()) {
            if (peek() == 'e' &&
                peek(1) == '!' &&
                peek(2) == '>') {
                
                advance();
                advance();
                advance();

                return true;
            }

            advance();

        }

        throw std::runtime_error(
            "Unterminated Sol comment."
        );
    }

    Token Lexer::makeToken(
        TokenKind kind,
        std::size_t start,
        std::size_t startLine,
        std::size_t startColumn
    ) {
        return {
            kind,
            source.substr(start, position - start),
            startLine,
            startColumn
        };
    }

    Token Lexer::simpleToken(TokenKind kind) {
        std::size_t start = position;
        std::size_t startLine = line;
        std::size_t startColumn = column;

        advance();

        return makeToken(
            kind,
            start,
            startLine,
            startColumn
        );
    }

    TokenKind Lexer::keywordKind(const std::string& word) const {
        static const std::unordered_map<std::string, TokenKind> keywords = {
            {"module", TokenKind::Module},
            {"use", TokenKind::Use},
            {"as", TokenKind::As},

            {"fn", TokenKind::Fn},
            {"loc", TokenKind::Loc},
            {"ert", TokenKind::Ert},
            {"stat", TokenKind::Stat},
            {"pub", TokenKind::Pub},
            {"priv", TokenKind::Priv},

            {"elem", TokenKind::Elem},
            {"struct", TokenKind::Struct},
            {"class", TokenKind::Class},
            {"enum", TokenKind::Enum},

            {"if", TokenKind::If},
            {"then", TokenKind::Then},
            {"else", TokenKind::Else},
            {"elseif", TokenKind::Elseif},
            {"end", TokenKind::End},

            {"while", TokenKind::While},
            {"for", TokenKind::For},
            {"until", TokenKind::Until},
            {"in", TokenKind::In},
            {"repeat", TokenKind::Repeat},
            {"until", TokenKind::Until},

            {"match", TokenKind::Match},
            {"case", TokenKind::Case},

            {"respond", TokenKind::Respond},
            {"return", TokenKind::Return},

            {"true", TokenKind::True},
            {"false", TokenKind::False},
            {"nil", TokenKind::Nil},

            {"try", TokenKind::Try},
            {"except", TokenKind::Except},
            {"finally", TokenKind::Finally},
            {"raise", TokenKind::Raise},

            {"compile", TokenKind::Compile},
            {"comptime", TokenKind::Comptime},

            {"bool", TokenKind::Type},
            {"char", TokenKind::Type},
            {"byte", TokenKind::Type},
            {"str", TokenKind::Type},

            {"int", TokenKind::Type},
            {"int8", TokenKind::Type},
            {"int16", TokenKind::Type},
            {"int32", TokenKind::Type},
            {"int64", TokenKind::Type},
            {"int128", TokenKind::Type},

            {"uint", TokenKind::Type},
            {"uint8", TokenKind::Type},
            {"uint16", TokenKind::Type},
            {"uint32", TokenKind::Type},
            {"uint64", TokenKind::Type},
            {"uint128", TokenKind::Type},

            {"float16", TokenKind::Type},
            {"float32", TokenKind::Type},
            {"float64", TokenKind::Type},
            {"float128", TokenKind::Type}
        };

        auto it = keywords.find(word);

        if (it != keywords.end())
            return it->second;

        return TokenKind::Identifier;
    }

    Token Lexer::identifier() {
        std::size_t start = position;
        std::size_t startLine = line;
        std::size_t startColumn  = column;

        while (std::isalnum(
            static_cast<unsigned char>(peek())
        ) || peek() == '_') {
            advance();
        }

        std::string word =
            source.substr(start, position - start);

        return {
            keywordKind(word),
            word,
            startLine,
            startColumn
        };
    }

    Token Lexer::typedLiteral() {
        std::size_t start = position;
        std::size_t startLine = line;
        std::size_t startColumn = column;

        char prefix = advance();

        char quote = peek();

        if (quote != '\'' && quote != '"') {
            throw std::runtime_error(
                "Expected quote after literal prefix."
            );
        }

        advance();

        while (!atEnd()) {
            char c = peek();

            if (c == '\\') {
                advance();

                if (!atEnd())
                    advance();

                continue;
            }

            if (c == quote) {
                advance();

                TokenKind kind;

                switch (prefix) {
                    case 't':
                        kind = TokenKind::Text;
                        break;

                    case 'n':
                        kind = TokenKind::Number;
                        break;

                    case 'b':
                        kind = TokenKind::Binary;
                        break;

                    case 'h':
                        kind = TokenKind::Hexadecimal;
                        break;

                    case 'c':
                        kind = TokenKind::Character;
                        break;

                    default:
                        kind = TokenKind::Identifier;
                        break;
                }

                return makeToken(
                    kind,
                    start,
                    startLine,
                    startColumn
                );
            }

            advance();
        }

        throw std::runtime_error(
            "Unterminated typed literal."
        );
    }

    std::vector<Token> Lexer::tokenize() {
        std::vector<Token> tokens;

        while (!atEnd()) {
            if (peek() == ' ' || peek() == '\t' || peek() == '\r') {
                advance();
                continue;
            }

            if (peek() == '\n') {
                const std::size_t start = position;
                const std::size_t startLine = line;
                const std::size_t startColumn = column;

                advance();

                if (!tokens.empty() &&
                    tokens.back() .kind == TokenKind::EOL) {
                    continue;
                }

                tokens.push_back(makeToken(
                    TokenKind::EOL,
                    start,
                    startLine,
                    startColumn
                ));

                continue;
            }

            // Comments
            if (peek() == '<' && peek(1) == '?' && peek(2) == 's') {
                skipComment();
                continue;
            }

            // Identifiers, keywords, and typed literals
            if (std::isalpha(static_cast<unsigned char>(peek())) ||
                peek() == '_') {
                
                    if ((peek() == 't' || peek() == 'n' ||
                         peek() == 'b' || peek() == 'h' ||
                         peek() == 'c') &&
                        (peek(1) == '"' || peek(1) == '\'')) {
                        tokens.push_back(typedLiteral());
                    } else {
                        tokens.push_back(identifier());
                    }

                    continue;
                }

                // Numbers
                if (std::isdigit(static_cast<unsigned char>(peek()))) {
                    tokens.push_back(identifier());
                    continue;
                }

                // Shift operators
                if (peek() == '<' && peek(1) == '<') {
                    const std::size_t start = position;
                    const std::size_t startLine = line;
                    const std::size_t startColumn = column;

                    advance();
                    advance();
                    advance();

                    tokens.push_back(makeToken(
                        TokenKind::ShiftLeft,
                        start,
                        startLine,
                        startColumn
                    ));
                    
                    continue;
                }

                // Less-than operators
                if (peek() == '<') {
                    if (peek(1) == '=') {
                        tokens.push_back(simpleToken(TokenKind::LessEqual));
                        advance();
                    } else {
                        tokens.push_back(simpleToken(TokenKind::Less));
                    }

                    continue;
                }

                // Greather-than operators
                if (peek() == '>') {
                    if (peek(1) == '=') {
                        tokens.push_back(simpleToken(TokenKind::GreaterEqual));
                        advance();
                    } else {
                        tokens.push_back(simpleToken(TokenKind::Greater));
                    }

                    continue;
                }

                // Statement terminator
                if (peek() == ';') {
                    tokens.push_back (simpleToken(TokenKind::EOL));
                    continue;
                }

            // Newline
            if (c == '\n') {
                advance();

                tokens.push_back({
                    TokenKind::EOL,
                    "\\n",
                    startLine,
                    startColumn
                });

                continue;
            }

            // Semicolon
            if (c == ';') {
                advance();

                tokens.push_back({
                    TokenKind::EOL,
                    ";",
                    startLine,
                    startColumn
                });

                continue;
            }

            // Identifier / keyword
            if (std::isalpha(
                static_cast<unsigned char>(c)
            ) || c == '_') {

                // Typed literal prefixes
                if ((c == 't' ||
                     c == 'n' ||
                     c == 'b' ||
                     c == 'h' ||
                     c == 'c') &&
                    (peek(1) == '\'' ||
                     peek(1) == '"')) {
                    
                    tokens.push_back(typedLiteral());
                } else {
                    tokens.push_back(identifier());
                }

                continue;
            }

            // Two character operators
            if (peek() == '\n') {
                const std::size_t start = position;
                const std::size_t startLine = line;
                const std::size_t startColumn = column;

                advance();

                // Don't create duplicate EOL tokens.
                if (!tokens.empty() && tokens.back().kind == TokenKind::EOL) {
                    continue;
                }

                tokens.push_back(makeToken(
                    TokenKind::EOL,
                    start,
                    startLine,
                    startColumn
                ));

                continue;
            }

            if (c == '-' && peek(1) == '>') {
                advance();
                advance();

                tokens.push_back({
                    TokenKind::Arrow,
                    "->",
                    startLine,
                    startColumn
                });

                continue;
            }

            if (c == '=' && peek(1) == '=') {
                advance();
                advance();

                tokens.push_back({
                    TokenKind::Equal,
                    "==",
                    startLine,
                    startColumn
                });

                continue;
            }

            if (c == '!' && peek(1) == '=') {
                advance();
                advance();

                tokens.push_back({
                    TokenKind::NotEqual,
                    "!=",
                    startLine,
                    startColumn
                });

                continue;
            }

            if (c == '<' && peek(1) == '=') {
                advance();
                advance();

                tokens.push_back({
                    TokenKind::LessEqual,
                    "<=",
                    startLine,
                    startColumn
                });

                continue;
            }

            if (c == '>' && peek(1) == '=') {
                advance();
                advance();

                tokens.push_back({
                    TokenKind::GreaterEqual,
                    ">=",
                    startLine,
                    startColumn
                });

                continue;
            }

            if (c == '&' && peek(1) == '&') {
                advance();
                advance();

                tokens.push_back({
                    TokenKind::LogicalAnd,
                    "&&",
                    startLine,
                    startColumn,
                });

                continue;
            }

            if (c == '|' && peek(1) == '|') {
                advance();
                advance();

                tokens.push_back({
                    TokenKind::LogicalOr,
                    "||",
                    startLine,
                    startColumn
                });

                continue;
            }

            if (c == '+' && peek(1) == '+') {
                advance();
                advance();

                tokens.push_back({
                    TokenKind::Increment,
                    "++",
                    startLine,
                    startColumn
                });

                continue;
            }

            if (c == '-' && peek(1) == '-') {
                advance();
                advance();

                tokens.push_back({
                    TokenKind::Decrement,
                    "--",
                    startLine,
                    startColumn
                });

                continue;
            }

            if (c == '+' && peek(1) == '=') {
                advance();
                advance();

                tokens.push_back({
                    TokenKind::PlusAssign,
                    "+=",
                    startLine,
                    startColumn
                });

                continue;
            }

            if (c == '-' && peek(1) == '=') {
                advance();
                advance();

                tokens.push_back({
                    TokenKind::MinusAssign,
                    "-=",
                    startLine,
                    startColumn
                });

                continue;
            }

            if (c == '*' && peek(1) == '=') {
                advance();
                advance();

                tokens.push_back({
                    TokenKind::MultiplyAssign,
                    "*=",
                    startLine,
                    startColumn
                });

                continue;
            }

            if (c == '/' && peek(1) == '=') {
                advance();
                advance();

                tokens.push_back({
                    TokenKind::DivideAssign,
                    "/=",
                    startLine,
                    startColumn
                });

                continue;
            }

            if (c == '%' && peek(1) == '=') {
                advance();
                advance();

                tokens.push_back({
                    TokenKind::ModuloAssign,
                    "%=",
                    startLine,
                    startColumn
                });

                continue;
            }

            if (peek() == '\\' && peek(1) == 'n') {
                const std::size_t start = position;
                const std::size_t startLine = line;
                const std::size_t startColumn = column;

                advance();
                advance();

                tokens.push_back(makeToken(
                    TokenKind::EOL,
                    start,
                    startLine,
                    startColumn
                ));

                continue;
            }

            if (peek() == '<') {
                if (peek(1) == '<') {
                    const std::size_t start = position;
                    const std::size_t startLine = line;
                    const std::size_t startColumn = column;

                    advance();
                    advance();

                    tokens.push_back(makeToken(
                        TokenKind::ShiftLeft,
                        start,
                        startLine,
                        startColumn
                    ));

                    continue;
                }

                if (peek(1) == '=') {
                    const std::size_t start = position;
                    const std::size_t startLine = line;
                    const std::size_t startColumn = column;

                    advance();
                    advance();

                    tokens.push_back(makeToken(
                        TokenKind::LessEqual,
                        start,
                        startLine,
                        startColumn
                    ));

                    continue;
                }

                tokens.push_back(simpleToken(TokenKind::Less));
                continue;
            }

            if (peek() == '>') {
                if (peek(1) == '>') {
                    const std::size_t start = position;
                    const std::size_t startLine = line;
                    const std::size_t startColumn = column;

                    advance();
                    advance();

                    tokens.push_back(makeToken(
                        TokenKind::ShiftRight,
                        start,
                        startLine,
                        startColumn
                    ));

                    continue;
                }

                if (peek(1) == '=') {
                    const std::size_t start = position;
                    const std::size_t startLine = line;
                    const std::size_t startColumn = column;

                    advance();
                    advance();

                    tokens.push_back(makeToken(
                        TokenKind::GreaterEqual,
                        start,
                        startLine,
                        startColumn
                    ));

                    continue;
                }

                tokens.push_back(simpleToken(TokenKind::Greater));
                continue;
            }

            // Single-character tokens
            TokenKind kind;

            switch(c) {
                case '+': kind = TokenKind::Plus; break;
                case '-': kind = TokenKind::Minus; break;
                case '*': kind = TokenKind::Multiply; break;
                case '/': kind = TokenKind::Divide; break;
                case '%': kind = TokenKind::Modulo; break;
                case '^': kind = TokenKind::Power; break;

                case '=': kind = TokenKind::Assign; break;

                case '<': kind = TokenKind::Less; break;
                case '>': kind = TokenKind::Greater; break;

                case '!': kind = TokenKind::LogicalNot; break;

                case '&': kind = TokenKind::BitAnd; break;
                case '|': kind = TokenKind::BitOr; break;
                case '~': kind = TokenKind::BitNot; break;

                case '?': kind = TokenKind::Question; break;

                case '(': kind = TokenKind::LeftParen; break;
                case ')': kind = TokenKind::RightParen; break;

                case '[': kind = TokenKind::LeftBracket; break;
                case ']': kind = TokenKind::RightBracket; break;

                case '{': kind = TokenKind::LeftBrace; break;
                case '}': kind = TokenKind::RightBrace; break;

                case ',': kind = TokenKind::Comma; break;
                case '.': kind = TokenKind::Dot; break;
                case ':': kind = TokenKind::Colon; break;
                case '@': kind = TokenKind::At; break;

                default:
                    throw std::runtime_error(
                        "Unknown character caught at line " +
                        std::to_string(line) +
                        ", column " + 
                        std::to_string(column)
                    );
            }

            advance();

            tokens.push_back({
                kind,
                source.substr(start, position - start),
                startLine,
                startColumn
            });
        }

        tokens.push_back({
            TokenKind::EndOfFile,
            "",
            line,
            column
        });

        return tokens;
    }
}