#include "Tokenizer.hpp"

#include "the_externs.hpp"

using std::stof;
using std::stoi;

Tokenizer::Tokenizer(unique_ptr<ifstream> &&file) : file{move(file)}
{
}

Tokenizer::~Tokenizer()
{
}

int Tokenizer::nextChar()
{
    return file->get();
}

TOKEN Tokenizer::next()
{
    if (tok_buffer.size())
    {
        CurTok = tok_buffer.front();
        tok_buffer.pop_front();
    }
    else
    {
        CurTok = gettok();
    }

    return CurTok;
}

void assert_tok_any(vector<TOKEN_TYPE> tok_types, string err_msg)
{
    for (auto tok_type : tok_types)
    {
        if (tok->CurTok.type == tok_type)
        {
            return;
        }
    }
    throw syntax_error(move(err_msg));
}

TOKEN Tokenizer::gettok()
{
    static int LastChar = ' ';
    static int NextChar = ' ';

    // Skip any whitespace.
    while (isspace(LastChar))
    {
        if ((LastChar == '\n') || (LastChar == '\r'))
        {
            lineNo++;
            columnNo = 1;
        }
        LastChar = nextChar();
        columnNo++;
    }

    if (isalpha(LastChar) ||
        (LastChar == '_')) // identifier: [a-zA-Z_][a-zA-Z_0-9]*
    {
        IdentifierStr = LastChar;
        columnNo++;

        while (isalnum((LastChar = nextChar())) || (LastChar == '_'))
        {
            IdentifierStr += LastChar;
            columnNo++;
        }

        if (IdentifierStr == "int")
        {
            return returnTok("int", INT_TOK);
        }
        if (IdentifierStr == "bool")
        {
            return returnTok("bool", BOOL_TOK);
        }
        if (IdentifierStr == "float")
        {
            return returnTok("float", FLOAT_TOK);
        }
        if (IdentifierStr == "void")
        {
            return returnTok("void", VOID_TOK);
        }
        if (IdentifierStr == "bool")
        {
            return returnTok("bool", BOOL_TOK);
        }
        if (IdentifierStr == "extern")
        {
            return returnTok("extern", EXTERN);
        }
        if (IdentifierStr == "if")
        {
            return returnTok("if", IF);
        }
        if (IdentifierStr == "else")
        {
            return returnTok("else", ELSE);
        }
        if (IdentifierStr == "while")
        {
            return returnTok("while", WHILE);
        }
        if (IdentifierStr == "return")
        {
            return returnTok("return", RETURN);
        }
        if (IdentifierStr == "true")
        {
            BoolVal = true;
            return retTokVal("true", BOOL_LIT, BoolVal);
        }
        if (IdentifierStr == "false")
        {
            BoolVal = false;
            return retTokVal("false", BOOL_LIT, BoolVal);
        }
        return returnTok(IdentifierStr.c_str(), IDENT);
    }

    if (LastChar == '=')
    {
        NextChar = nextChar();
        if (NextChar == '=') // EQ: ==
        {
            LastChar = nextChar();
            columnNo += 2;
            return returnTok("==", EQ);
        }
        else
        {
            LastChar = NextChar;
            columnNo++;
            return returnTok("=", ASSIGN);
        }
    }

    if (LastChar == '{')
    {
        LastChar = nextChar();
        columnNo++;
        return returnTok("{", LBRA);
    }
    if (LastChar == '}')
    {
        LastChar = nextChar();
        columnNo++;
        return returnTok("}", RBRA);
    }
    if (LastChar == '(')
    {
        LastChar = nextChar();
        columnNo++;
        return returnTok("(", LPAR);
    }
    if (LastChar == ')')
    {
        LastChar = nextChar();
        columnNo++;
        return returnTok(")", RPAR);
    }
    if (LastChar == ';')
    {
        LastChar = nextChar();
        columnNo++;
        return returnTok(";", SC);
    }
    if (LastChar == ',')
    {
        LastChar = nextChar();
        columnNo++;
        return returnTok(",", COMMA);
    }

    if (isdigit(LastChar) || (LastChar == '.')) // Number: [0-9]+.
    {
        string NumStr;

        if (LastChar == '.') // Floatingpoint Number: .[0-9]+
        {
            do
            {
                NumStr += LastChar;
                LastChar = nextChar();
                columnNo++;
            } while (isdigit(LastChar));

            FloatVal = stof(NumStr, nullptr);
            return retTokVal(NumStr, FLOAT_LIT, FloatVal);
        }
        else
        {
            do // Start of Number: [0-9]+
            {
                NumStr += LastChar;
                LastChar = nextChar();
                columnNo++;
            } while (isdigit(LastChar));

            if (LastChar == '.') // Floatingpoint Number: [0-9]+.[0-9]+)
            {
                do
                {
                    NumStr += LastChar;
                    LastChar = nextChar();
                    columnNo++;
                } while (isdigit(LastChar));

                FloatVal = stof(NumStr, nullptr);
                return retTokVal(NumStr, FLOAT_LIT, FloatVal);
            }
            else // Integer : [0-9]+
            {
                IntVal = stoi(NumStr, nullptr);
                return retTokVal(NumStr, INT_LIT, IntVal);
            }
        }
    }

    if (LastChar == '&')
    {
        NextChar = nextChar();
        if (NextChar == '&') // AND: &&
        {
            LastChar = nextChar();
            columnNo += 2;
            return returnTok("&&", AND);
        }
        else
        {
            LastChar = NextChar;
            columnNo++;
            return returnTok("&", int('&'));
        }
    }

    if (LastChar == '|')
    {
        NextChar = nextChar();
        if (NextChar == '|') // OR: ||
        {
            LastChar = nextChar();
            columnNo += 2;
            return returnTok("||", OR);
        }
        else
        {
            LastChar = NextChar;
            columnNo++;
            return returnTok("|", int('|'));
        }
    }

    if (LastChar == '!')
    {
        NextChar = nextChar();
        if (NextChar == '=') // NE: !=
        {
            LastChar = nextChar();
            columnNo += 2;
            return returnTok("!=", NE);
        }
        else
        {
            LastChar = NextChar;
            columnNo++;
            return returnTok("!", NOT);
        }
    }

    if (LastChar == '<')
    {
        NextChar = nextChar();
        if (NextChar == '=') // LE: <=
        {
            LastChar = nextChar();
            columnNo += 2;
            return returnTok("<=", LE);
        }
        else
        {
            LastChar = NextChar;
            columnNo++;
            return returnTok("<", LT);
        }
    }

    if (LastChar == '>')
    {
        NextChar = nextChar();
        if (NextChar == '=') // GE: >=
        {
            LastChar = nextChar();
            columnNo += 2;
            return returnTok(">=", GE);
        }
        else
        {
            LastChar = NextChar;
            columnNo++;
            return returnTok(">", GT);
        }
    }

    if (LastChar == '/') // could be division or could be the start of a comment
    {
        LastChar = nextChar();
        columnNo++;
        if (LastChar == '/') // definitely a comment
        {
            do
            {
                LastChar = nextChar();
                columnNo++;
            } while (LastChar != EOF && LastChar != '\n' && LastChar != '\r');

            if (LastChar != EOF)
            {
                return gettok();
            }
        }
        else
        {
            return returnTok("/", DIV);
        }
    }

    // Check for end of file.  Don't eat the EOF.
    if (LastChar == EOF)
    {
        columnNo++;
        return returnTok("0", EOF_TOK);
    }

    // Otherwise, just return the character as its ascii value.
    int ThisChar = LastChar;
    string s(1, ThisChar);

    LastChar = nextChar();
    columnNo++;
    return returnTok(s, int(ThisChar));
}

void assert_tok(TOKEN_TYPE tok_type, string err_msg)
{
    if (tok->CurTok.type != tok_type)
    {
        throw syntax_error(move(err_msg));
    }
}

void Tokenizer::put_back(TOKEN tok)
{
    tok_buffer.push_front(tok);
}

TOKEN Tokenizer::returnTok(string lexVal, int tok_type)
{
    TOKEN return_tok;

    return_tok.lexeme = lexVal;
    return_tok.type = tok_type;
    return_tok.lineNo = lineNo;
    return_tok.columnNo = columnNo - lexVal.length() - 1;
    return return_tok;
}

template <typename V>
TokenWithValue<V> Tokenizer::retTokVal(string lexVal, int tok_type, V value)
{
    TokenWithValue<V> return_tok;

    return_tok.lexeme = lexVal;
    return_tok.type = tok_type;
    return_tok.lineNo = lineNo;
    return_tok.columnNo = columnNo - lexVal.length() - 1;
    return_tok.value = value;
    return return_tok;
}
