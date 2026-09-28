#include "sol/Token.h"

namespace sol {
    
    const char* tokenKindName(TokenKind kind) {
        switch(kind) {
            case TokenKind::EndOfFile: return "EOF";

            case TokenKind::Identifier: return "IDENTIFIER";

            case TokenKind::Number: return "NUMBER";
            case TokenKind::Text: return "TEXT";
            case TokenKind::Character: return "CHARACTER";
            case TokenKind::Binary: return "BINARY";
            case TokenKind::Hexadecimal: return "HEXADECIMAL";

            case TokenKind::EOL: return "EOL";

            case TokenKind::Module: return "MODULE";
            case TokenKind::Use: return "USE";
            case TokenKind::As: return "AS";

            case TokenKind::Fn: return "FN";
            case TokenKind::Loc: return "LOC";
            case TokenKind::Ert: return "ERT";
            case TokenKind::Stat: return "STAT";
            case TokenKind::Pub: return "PUB";
            case TokenKind::Priv: return "PRIV";

            case TokenKind::Elem: return "ELEM";
            case TokenKind::Struct: return "STRUCT";
            case TokenKind::Class: return "CLASS";
            case TokenKind::Enum: return "ENUM";

            case TokenKind::If: return "IF";
            case TokenKind::Then: return "THEN";
            case TokenKind::Else: return "ELSE";
            case TokenKind::Elseif: return "ELSEIF";
            case TokenKind::End: return "END";

            case TokenKind::While: return "WHILE";
            case TokenKind::For: return "FOR";
            case TokenKind::In: return "IN";
            case TokenKind::Repeat: return "REPEAT";
            case TokenKind::Until: return "UNTIL";

            case TokenKind::Match: return "MATCH";
            case TokenKind::Case: return "CASE";

            case TokenKind::Respond: return "RESPOND";
            case TokenKind::Return: return "RETURN";

            case TokenKind::True: return "TRUE";
            case TokenKind::False: return "FALSE";
            case TokenKind::Nil: return "NIL";

            case TokenKind::Try: return "TRY";
            case TokenKind::Except: return "EXCEPT";
            case TokenKind::Finally: return "FINALLY";
            case TokenKind::Raise: return "RAISE";

            case TokenKind::Compile: return "COMPILE";
            case TokenKind::Comptime: return "COMPTIME";

            case TokenKind::Type: return "TYPE";

            case TokenKind::Plus: return "PLUS";
            case TokenKind::Minus: return "MINUS";
            case TokenKind::Multiply: return "MULTIPLY";
            case TokenKind::Divide: return "DIVIDE";
            case TokenKind::Modulo: return "MODULO";
            case TokenKind::Power: return "POWER";

            case TokenKind::Assign: return "ASSIGN";
            case TokenKind::Equal: return "EQUAL";
            case TokenKind::NotEqual: return "NOT_EQUAL";

            case TokenKind::Less: return "LESS";
            case TokenKind::LessEqual: return "LESS_EQUAL";
            case TokenKind::Greater: return "GREATER";
            case TokenKind::GreaterEqual: return "GREATER_EQUAL";

            case TokenKind::LogicalAnd: return "LOGICAL_AND";
            case TokenKind::LogicalOr: return "LOGICAL_OR";
            case TokenKind::LogicalNot: return "LOGICAL_NOT";

            case TokenKind::BitAnd: return "BIT_AND";
            case TokenKind::BitOr: return "BIT_OR";
            case TokenKind::BitNot: return "BIT_NOT";

            case TokenKind::ShiftLeft: return "SHIFT_LEFT";
            case TokenKind::ShiftRight: return "SHIFT_RIGHT";

            case TokenKind::PlusAssign: return "PLUS_ASSIGN";
            case TokenKind::MinusAssign: return "MINUS_ASSIGN";
            case TokenKind::MultiplyAssign: return "MULTIPLY_ASSIGN";
            case TokenKind::DivideAssign: return "DIVIDE_ASSIGN";
            case TokenKind::ModuloAssign: return "MODULO_ASSIGN";

            case TokenKind::Increment: return "INCREMENT";
            case TokenKind::Decrement: return "DECREMENT";

            case TokenKind::Arrow: return "ARROW";

            case TokenKind::Question: return "QUESTION";

            case TokenKind::LeftParen: return "LEFT_PAREN";
            case TokenKind::RightParen: return "RIGHT_PARENT";

            case TokenKind::LeftBracket: return "LEFT_BRACKET";
            case TokenKind::RightBracket: return "RIGHT_BRACKET";

            case TokenKind::LeftBrace: return "LEFT_BRACE";
            case TokenKind::RightBrace: return "RIGHT_BRACE";

            case TokenKind::Comma: return "COMMA";
            case TokenKind::Dot: return "DOT";
            case TokenKind::Colon: return "COLON";
            case TokenKind::At: return "AT";
        }

        return "UNKNOWN";
    }
}