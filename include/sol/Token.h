#pragma once

#include <cstddef>
#include <string>

namespace sol {
    enum class TokenKind {
        EndOfFile,

        Identifier,

        Number,
        Text,
        Character,
        Binary,
        Hexadecimal,

        EOL,

        // Keywords
        Module,
        Use,
        As,

        Fn,
        Loc,
        Ert,
        Stat,
        Pub,
        Priv,

        Elem,
        Struct,
        Class,
        Enum,

        If,
        Then,
        Else,
        Elseif,
        End,

        While,
        For,
        In,
        Repeat,
        Until,

        Match,
        Case,

        Respond,
        Return,

        True,
        False,
        Nil,

        Try,
        Except,
        Finally,
        Raise,

        Compile,
        Comptime,

        // Types
        Type,

        // Operators
        Plus,
        Minus,
        Multiply,
        Divide,
        Modulo,
        Power,

        Assign,
        Equal,
        NotEqual,

        Less,
        LessEqual,
        Greater,
        GreaterEqual,

        LogicalAnd,
        LogicalOr,
        LogicalNot,

        BitAnd,
        BitOr,
        BitNot,

        ShiftLeft,
        ShiftRight,

        PlusAssign,
        MinusAssign,
        MultiplyAssign,
        DivideAssign,
        ModuloAssign,

        Increment,
        Decrement,

        Arrow,

        Question,

        // Punctuation
        LeftParen,
        RightParen,

        LeftBracket,
        RightBracket,

        LeftBrace,
        RightBrace,

        Comma,
        Dot,
        Colon,
        At
    };

    struct Token {
        TokenKind kind;
        std::string lexeme;
        std::size_t line;
        std::size_t column;
    };

    const char* tokenKindName(TokenKind kind);
}