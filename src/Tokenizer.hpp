#pragma once

#include <deque>
#include <vector>

#include "Token.hpp"
#include "TokenType.hpp"

using std::deque;
using std::vector;

extern string IdentifierStr; // Filled in if IDENT
extern int IntVal;           // Filled in if INT_LIT
extern bool BoolVal;         // Filled in if BOOL_LIT
extern float FloatVal;       // Filled in if FLOAT_LIT
extern string StringVal;     // Filled in if String Literal
extern int lineNo, columnNo;

int nextChar();

TOKEN returnTok(string lexVal, int tok_type);

// Read file line by line -- or look for \n and if found add 1 to line number
// and reset column number to 0
/// gettok - Return the next token from standard input.
TOKEN gettok();

TOKEN getNextToken();

void putBackToken(TOKEN tok);

void assert_tok(TOKEN_TYPE tok_type, string err_msg);

void assert_tok_any(vector<TOKEN_TYPE> tok_types, string err_msg);
