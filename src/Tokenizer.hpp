#pragma once

#include <deque>
#include <vector>

#include "Token.hpp"
#include "TokenType.hpp"

using std::deque;
using std::vector;

class Tokenizer
{
    deque<TOKEN> tok_buffer;

    int nextChar();

    TOKEN returnTok(string lexVal, int tok_type);

    // Read file line by line -- or look for \n and if found add 1 to line
    // number and reset column number to 0
    /// gettok - Return the next token from standard input.
    TOKEN gettok();

  public:
    string IdentifierStr; // Filled in if IDENT
    int IntVal;           // Filled in if INT_LIT
    bool BoolVal;         // Filled in if BOOL_LIT
    float FloatVal;       // Filled in if FLOAT_LIT
    string StringVal;     // Filled in if String Literal
    int lineNo, columnNo;

    /// CurTok/next - Provide a simple token buffer.  CurTok is the
    /// current token the parser is looking at.  next reads another
    /// token from the lexer and updates CurTok with its results.
    TOKEN CurTok;

    Tokenizer(/* args */);
    ~Tokenizer();

    TOKEN next();

    void put_back(TOKEN tok);
};

void assert_tok(TOKEN_TYPE tok_type, string err_msg);

void assert_tok_any(vector<TOKEN_TYPE> tok_types, string err_msg);
