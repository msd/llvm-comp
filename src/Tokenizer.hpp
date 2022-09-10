#pragma once

#include <deque>
#include <vector>

#include "Token.hpp"
#include "TokenType.hpp"

using std::deque;
using std::vector;

class Tokenizer
{
  public:
    string IdentifierStr; // Filled in if IDENT
    int IntVal;           // Filled in if INT_LIT
    bool BoolVal;         // Filled in if BOOL_LIT
    float FloatVal;       // Filled in if FLOAT_LIT
    string StringVal;     // Filled in if String Literal
    int lineNo, columnNo;

    /// CurTok/getNextToken - Provide a simple token buffer.  CurTok is the
    /// current token the parser is looking at.  getNextToken reads another
    /// token from the lexer and updates CurTok with its results.
    TOKEN CurTok;

    deque<TOKEN> tok_buffer;

    Tokenizer(/* args */);
    ~Tokenizer();

    int nextChar();

    TOKEN returnTok(string lexVal, int tok_type);

    // Read file line by line -- or look for \n and if found add 1 to line
    // number and reset column number to 0
    /// gettok - Return the next token from standard input.
    TOKEN gettok();

    TOKEN getNextToken();

    void putBackToken(TOKEN tok);
};

void assert_tok(TOKEN_TYPE tok_type, string err_msg);

void assert_tok_any(vector<TOKEN_TYPE> tok_types, string err_msg);
