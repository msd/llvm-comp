#include "Tokenizer.hpp"

#include <cstdio>

#include "the_externs.hpp"
#include "my_errors.hpp"

string IdentifierStr; // Filled in if IDENT
int IntVal; // Filled in if INT_LIT
bool BoolVal; // Filled in if BOOL_LIT
float FloatVal; // Filled in if FLOAT_LIT
string StringVal; // Filled in if String Literal
int lineNo, columnNo;

deque<TOKEN> tok_buffer;

int nextChar()
{
    return getc(pFile);
}

TOKEN getNextToken()
{
	if (tok_buffer.size() == 0)
	{
		tok_buffer.push_back(gettok());
	}

	TOKEN temp = tok_buffer.front();

	tok_buffer.pop_front();

	return CurTok = temp;
}

void assert_tok_any(vector<TOKEN_TYPE> tok_types, string err_msg)
{
	for (auto tok_type: tok_types)
	{
		if (CurTok.type == tok_type)
		{
			return;
		}
	}
	throw syntax_error(move(err_msg));
}

TOKEN gettok()
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
	    (LastChar == '_'))                    // identifier: [a-zA-Z_][a-zA-Z_0-9]*
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
			return returnTok("true", BOOL_LIT);
		}
		if (IdentifierStr == "false")
		{
			BoolVal = false;
			return returnTok("false", BOOL_LIT);
		}
		return returnTok(IdentifierStr.c_str(), IDENT);
	}

	if (LastChar == '=')
	{
		NextChar = nextChar();
		if (NextChar == '=')             // EQ: ==
		{
			LastChar  = nextChar();
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

	if (isdigit(LastChar) || (LastChar == '.'))      // Number: [0-9]+.
	{
		string NumStr;

		if (LastChar == '.')             // Floatingpoint Number: .[0-9]+
		{
			do
			{
				NumStr  += LastChar;
				LastChar = nextChar();
				columnNo++;
			} while (isdigit(LastChar));

			FloatVal = strtof(NumStr.c_str(), nullptr);
			return returnTok(NumStr, FLOAT_LIT);
		}
		else
		{
			do                  // Start of Number: [0-9]+
			{
				NumStr  += LastChar;
				LastChar = nextChar();
				columnNo++;
			} while (isdigit(LastChar));

			if (LastChar == '.')                  // Floatingpoint Number: [0-9]+.[0-9]+)
			{
				do
				{
					NumStr  += LastChar;
					LastChar = nextChar();
					columnNo++;
				} while (isdigit(LastChar));

				FloatVal = strtof(NumStr.c_str(), nullptr);
				return returnTok(NumStr, FLOAT_LIT);
			}
			else                    // Integer : [0-9]+
			{
				IntVal = strtod(NumStr.c_str(), nullptr);
				return returnTok(NumStr, INT_LIT);
			}
		}
	}

	if (LastChar == '&')
	{
		NextChar = nextChar();
		if (NextChar == '&')             // AND: &&
		{
			LastChar  = nextChar();
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
		if (NextChar == '|')             // OR: ||
		{
			LastChar  = nextChar();
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
		if (NextChar == '=')             // NE: !=
		{
			LastChar  = nextChar();
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
		if (NextChar == '=')             // LE: <=
		{
			LastChar  = nextChar();
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
		if (NextChar == '=')             // GE: >=
		{
			LastChar  = nextChar();
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

	if (LastChar == '/')        // could be division or could be the start of a comment
	{
		LastChar = nextChar();
		columnNo++;
		if (LastChar == '/')             // definitely a comment
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
	int         ThisChar = LastChar;
	string s(1, ThisChar);

	LastChar = nextChar();
	columnNo++;
	return returnTok(s, int(ThisChar));
}

void assert_tok(TOKEN_TYPE tok_type, string err_msg)
{
	if (CurTok.type != tok_type)
	{
		throw syntax_error(move(err_msg));
	}
}

void putBackToken(TOKEN tok)
{
	tok_buffer.push_front(tok);
}

TOKEN returnTok(string lexVal, int tok_type)
{
	TOKEN return_tok;

	return_tok.lexeme   = lexVal;
	return_tok.type     = tok_type;
	return_tok.lineNo   = lineNo;
	return_tok.columnNo = columnNo - lexVal.length() - 1;
	return return_tok;
}
