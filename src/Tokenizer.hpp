#pragma once

#include "TokenType.hpp"
#include "Token.hpp"
#include <deque>
#include <vector>

using std::deque;
using std::vector;

static string IdentifierStr; // Filled in if IDENT
static int IntVal; // Filled in if INT_LIT
static bool BoolVal; // Filled in if BOOL_LIT
static float FloatVal; // Filled in if FLOAT_LIT
static string StringVal; // Filled in if String Literal
static int lineNo, columnNo;

static deque<TOKEN> tok_buffer;

extern int nextChar();

static TOKEN returnTok(string lexVal, int tok_type)
{
	TOKEN return_tok;

	return_tok.lexeme   = lexVal;
	return_tok.type     = tok_type;
	return_tok.lineNo   = lineNo;
	return_tok.columnNo = columnNo - lexVal.length() - 1;
	return return_tok;
}

// Read file line by line -- or look for \n and if found add 1 to line number
// and reset column number to 0
/// gettok - Return the next token from standard input.
TOKEN gettok();

TOKEN getNextToken();

void putBackToken(TOKEN tok);

void assert_tok(TOKEN_TYPE tok_type, string err_msg);

void assert_tok_any(vector<TOKEN_TYPE> tok_types, string err_msg);
