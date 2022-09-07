#include <iostream>
#include <map>
#include <queue>
#include <stack>

#include "llvm/IR/IRBuilder.h"
#include "llvm/IR/Verifier.h"
#include "llvm/Support/FileSystem.h"

#include "AssignmentLHS.hpp"
#include "StmtListNode.hpp"
#include "NegationNode.hpp"
#include "ProgramNode.hpp"
#include "DeclNode.hpp"
#include "ParenExprNode.hpp"
#include "AssignmentExpr.hpp"
#include "AssignmentLHS.hpp"
#include "FunctionSignature.hpp"
#include "FunDeclNode.hpp"
#include "ASTnode.hpp"
#include "ValidTypes.hpp"
#include "BoolNode.hpp"
#include "FunCallNode.hpp"
#include "NotNode.hpp"
#include "VarExprNode.hpp"
#include "IntNode.hpp"
#include "FloatNode.hpp"
#include "Ops.hpp"
#include "DisCon.hpp"
#include "NullStmt.hpp"
#include "IfStmt.hpp"
#include "IfWithElseNode.hpp"
#include "ReturnNothingNode.hpp"
#include "ReturnValueNode.hpp"
#include "WhileStmt.hpp"
#include "LocalDeclsNode.hpp"
#include "coercion.hpp"

#include "my_errors.hpp"

using namespace llvm;
using namespace llvm::sys;

using std::cerr;
using std::cout;
using std::cout;
using std::deque;
using std::endl;
using std::exception;
using std::make_unique;
using std::map;
using std::move;
using std::stack;
using std::string;
using std::unique_ptr;
using std::vector;

FILE *pFile;

// todo type checking conforming i.e. integers + floating point -> float + float (coercion)s|

//===----------------------------------------------------------------------===//
// Lexer
//===----------------------------------------------------------------------===//

// The lexer returns one of these for known things.
enum TOKEN_TYPE
{
	IDENT     = -1,         // [a-zA-Z_][a-zA-Z_0-9]*
	ASSIGN    = int('='),   // '='

	// delimiters
	LBRA      = int('{'),  // left brace
	RBRA      = int('}'),  // right brace
	LPAR      = int('('),  // left parenthesis
	RPAR      = int(')'),  // right parenthesis
	SC        = int(';'),  // semicolon
	COMMA     = int(','),  // comma

	// types
	INT_TOK   = -2,      // "int"
	VOID_TOK  = -3,      // "void"
	FLOAT_TOK = -4,      // "float"
	BOOL_TOK  = -5,      // "bool"

	// keywords
	EXTERN    = -6,    // "extern"
	IF        = -7,    // "if"
	ELSE      = -8,    // "else"
	WHILE     = -9,    // "while"
	RETURN    = -10,   // "return"
	// TRUE   = -12,     // "true"
	// FALSE   = -13,     // "false"

	// literals
	INT_LIT   = -14,      // [0-9]+
	FLOAT_LIT = -15,      // [0-9]+.[0-9]+
	BOOL_LIT  = -16,      // "true" or "false" key words

	// logical operators
	AND       = -17, // "&&"
	OR        = -18, // "||"

	// operators
	PLUS      = int('+'),    // addition or unary plus
	MINUS     = int('-'),    // substraction or unary negative
	ASTERISK  = int('*'),    // multiplication
	DIV       = int('/'),    // division
	MOD       = int('%'),    // modular
	NOT       = int('!'),    // unary negation

	// comparison operators
	EQ        = -19,      // equal
	NE        = -20,      // not equal
	LE        = -21,      // less than or equal to
	LT        = int('<'), // less than
	GE        = -23,      // greater than or equal to
	GT        = int('>'), // greater than

	// special tokens
	EOF_TOK   = 0,    // signal end of file

	// invalid
	INVALID   = -100    // signal invalid token
};

static string IdentifierStr; // Filled in if IDENT
static int         IntVal;        // Filled in if INT_LIT
static bool        BoolVal;       // Filled in if BOOL_LIT
static float       FloatVal;      // Filled in if FLOAT_LIT
static string StringVal;     // Filled in if String Literal
static int         lineNo, columnNo;

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
static TOKEN gettok()
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
		LastChar = getc(pFile);
		columnNo++;
	}

	if (isalpha(LastChar) ||
	    (LastChar == '_'))                    // identifier: [a-zA-Z_][a-zA-Z_0-9]*
	{
		IdentifierStr = LastChar;
		columnNo++;

		while (isalnum((LastChar = getc(pFile))) || (LastChar == '_'))
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
		NextChar = getc(pFile);
		if (NextChar == '=')             // EQ: ==
		{
			LastChar  = getc(pFile);
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
		LastChar = getc(pFile);
		columnNo++;
		return returnTok("{", LBRA);
	}
	if (LastChar == '}')
	{
		LastChar = getc(pFile);
		columnNo++;
		return returnTok("}", RBRA);
	}
	if (LastChar == '(')
	{
		LastChar = getc(pFile);
		columnNo++;
		return returnTok("(", LPAR);
	}
	if (LastChar == ')')
	{
		LastChar = getc(pFile);
		columnNo++;
		return returnTok(")", RPAR);
	}
	if (LastChar == ';')
	{
		LastChar = getc(pFile);
		columnNo++;
		return returnTok(";", SC);
	}
	if (LastChar == ',')
	{
		LastChar = getc(pFile);
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
				LastChar = getc(pFile);
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
				LastChar = getc(pFile);
				columnNo++;
			} while (isdigit(LastChar));

			if (LastChar == '.')                  // Floatingpoint Number: [0-9]+.[0-9]+)
			{
				do
				{
					NumStr  += LastChar;
					LastChar = getc(pFile);
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
		NextChar = getc(pFile);
		if (NextChar == '&')             // AND: &&
		{
			LastChar  = getc(pFile);
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
		NextChar = getc(pFile);
		if (NextChar == '|')             // OR: ||
		{
			LastChar  = getc(pFile);
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
		NextChar = getc(pFile);
		if (NextChar == '=')             // NE: !=
		{
			LastChar  = getc(pFile);
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
		NextChar = getc(pFile);
		if (NextChar == '=')             // LE: <=
		{
			LastChar  = getc(pFile);
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
		NextChar = getc(pFile);
		if (NextChar == '=')             // GE: >=
		{
			LastChar  = getc(pFile);
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
		LastChar = getc(pFile);
		columnNo++;
		if (LastChar == '/')             // definitely a comment
		{
			do
			{
				LastChar = getc(pFile);
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

	LastChar = getc(pFile);
	columnNo++;
	return returnTok(s, int(ThisChar));
}


//===----------------------------------------------------------------------===//
// Parser
//===----------------------------------------------------------------------===//

/// CurTok/getNextToken - Provide a simple token buffer.  CurTok is the current
/// token the parser is looking at.  getNextToken reads another token from the
/// lexer and updates CurTok with its results.
TOKEN CurTok;
static deque<TOKEN> tok_buffer;

static TOKEN getNextToken()
{
	if (tok_buffer.size() == 0)
	{
		tok_buffer.push_back(gettok());
	}

	TOKEN temp = tok_buffer.front();

	tok_buffer.pop_front();

	return CurTok = temp;
}


static void putBackToken(TOKEN tok)
{
	tok_buffer.push_front(tok);
}


//===----------------------------------------------------------------------===//
// AST nodes
//===----------------------------------------------------------------------===//

LLVMContext TheContext;
IRBuilder<> Builder(TheContext);
// Make the module, which holds all the code.
unique_ptr<Module> TheModule = make_unique<Module>("mini-c", TheContext);
map<string, FunctionSignature *> ExternedFunctions;
map<string, FunDeclNode *> DefinedFunctions;
stack<VariableScope *> ActiveScopes;

static unique_ptr<ASTnode> parse_stmt();
static unique_ptr<ASTnode> parse_stmt_list();

//===----------------------------------------------------------------------===//
// Recursive Descent Parser - Function call for each production
//===----------------------------------------------------------------------===//

/* Add function calls for each production */

static unique_ptr<ASTnode> parse_rval();

static void assert_tok_any(vector<TOKEN_TYPE> tok_types, string err_msg)
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


static void assert_tok(TOKEN_TYPE tok_type, string err_msg)
{
	if (CurTok.type != tok_type)
	{
		throw syntax_error(move(err_msg));
	}
}


static unique_ptr<ASTnode> LogError(string err_txt)
{
	cerr << "line " << CurTok.lineNo << " column "
	          << CurTok.columnNo << " error: " << err_txt << '\n';
	return nullptr;
}


static auto parse_paren_expr()
{
	assert_tok(LPAR, "Expected opening parenthesis '('");
	getNextToken();      // Consume open parenthesis '('
	auto r = parse_rval();

	assert_tok(RPAR, "Expected closing parenthesis ')' after rvalue");
	getNextToken();      // Consume closing parenthesis ')'
	return make_unique<ParenExprNode>(move(r));
}


static auto parse_neg_term()
{
	assert_tok(MINUS, "Expected minus '-'");
	getNextToken();      // Consume minus '-'
	return make_unique<NegationNode>(parse_rval());
}


static unique_ptr<ASTnode> parse_expr()
{
	if (CurTok.type == IDENT)
	{
		auto ident_tok  = CurTok;
		auto ident_name = CurTok.lexeme;
		getNextToken();           // Consume IDENT
		auto peek = CurTok;
		if (CurTok.type == ASSIGN)
		{
			getNextToken();                // Consume '='
			auto lhs = make_unique<AssignmentLHS>(ident_name);
			return make_unique<AssignmentExpr>(ActiveScopes.top(), move(lhs), parse_expr());
		}
		else
		{
			// putting back the token doesn't actually restore any of the state values :(
			// assert(peek.type != IDENT); should be enforced to preserve the
			// IdentifierStr but since IdentifierStr is also equal to the
			// lexeme attribute it should be no problem. Also two consecutive
			// identifier tokens don't constitute a valid sentece of this
			// grammar hence should emit an error further down the recursion
			// anyway
			putBackToken(peek);
			CurTok        = ident_tok;
			IdentifierStr = ident_name;
			return parse_rval();
		}
	}
	else
	{
		return parse_rval();
	}
}


static auto parse_args()
{
	auto args = vector<unique_ptr<ASTnode> >();

	while (CurTok.type != RPAR)
	{
		// todo check if current token is type
		args.push_back(parse_expr());
		if (CurTok.type == COMMA)
		{
			getNextToken();                // Consume ","
		}
	}
	return args;
}


static auto parse_rval_FLOAT_LIT()
{
	assert_tok(FLOAT_LIT, "Expected float literal");
	auto r = make_unique<FloatNode>(FloatVal);

	getNextToken();      // Consume FLOAT_LIT
	return r;
}


static auto parse_rval_INT_LIT()
{
	assert_tok(INT_LIT, "Expected int literal");
	auto r = make_unique<IntNode>(IntVal);

	getNextToken();      // Consume INT_LIT
	return r;
}


static auto parse_rval_BOOL_LIT()
{
	assert_tok(BOOL_LIT, "Expected bool literal");
	auto r = make_unique<BoolNode>(BoolVal);

	getNextToken();      // Consume BOOL_LIT
	return r;
}


static unique_ptr<RvalNode> parse_rval_var_or_fun()
{
	assert_tok(IDENT, "Expected identifier");
	auto ident_name = CurTok.lexeme;

	getNextToken();          // Consume IDENT
	if (CurTok.type == LPAR) // Parse Function call
	{
		auto fun_call = make_unique<FunCallNode>(ident_name);
		assert_tok(LPAR, "This should not happen :(");
		getNextToken();           // Consume opening paren "("
		fun_call->setSubs(parse_args());
		assert_tok(RPAR, "Expected closing right parenthesis ')' after function arguments");
		getNextToken();           // Consume closing paren ')'
		return fun_call;
	}
	else      // Parse Var
	{
		// Check variable is declared
		auto var_decl = ActiveScopes.top()->getDecl(ident_name);
		if (var_decl)
		{
			return make_unique<VarExprNode>(ActiveScopes.top(), ident_name, var_decl);
		}
		else // variable is not declared
		{
			throw semantic_error("Variable " + ident_name + " used but not declared");
		}
	}
}


static auto parse_not_term()
{
	assert_tok(NOT, "Expected '!'");
	getNextToken();      // Consume "!"
	return make_unique<NotNode>(parse_rval());
}


static unique_ptr<ASTnode> parse_rval_term()
{
	switch (CurTok.type)
	{
	case INT_LIT:
		return parse_rval_INT_LIT();

	case BOOL_LIT:
		return parse_rval_BOOL_LIT();

	case FLOAT_LIT:
		return parse_rval_FLOAT_LIT();

	case NOT:
		return parse_not_term();

	case MINUS:
		return parse_neg_term();

	case LPAR:
		return parse_paren_expr();

	default:
		return parse_rval_var_or_fun();
	}
}


static auto parse_rval_multiplication()
{
	unique_ptr<ASTnode> to_be_returned = parse_rval_term();
	bool loop           = true;

	while (loop)
	{
		char l_type, r_type;
		unique_ptr<ASTnode> rhs;
		switch (CurTok.type)
		{
		case ASTERISK:
			getNextToken();                        // consume "*"
			to_be_returned = make_unique<OpMULT>(move(to_be_returned), parse_rval_term());
			break;

		case DIV:
			getNextToken();                        // consume "/"
			to_be_returned = make_unique<OpDIV>(move(to_be_returned), parse_rval_term());
			break;

		case MOD:
			getNextToken();                        // consume "%"
			l_type = dynamic_cast<RvalNode *>(to_be_returned.get())->expr_type();
			r_type = dynamic_cast<RvalNode *>(rhs.get())->expr_type();
			rhs    = parse_rval_term();
			if ((l_type == FLOAT_TYPE) || (r_type == FLOAT_TYPE))
			{
				throw semantic_error("Invalid operands of '%' (have '" + type_to_str(l_type) + "' and '" + type_to_str(l_type) + "')");
			}
			to_be_returned = make_unique<OpMODULO>(move(to_be_returned), parse_rval_term());
			break;

		default:
			loop = false;
		}
	}
	return to_be_returned;
}


// todo refactor to make more sense (parses also subtraction)
static auto parse_rval_addition()
{
	auto to_be_returned = parse_rval_multiplication();
	bool loop           = true;

	while (loop)
	{
		switch (CurTok.type)
		{
		case PLUS:
			getNextToken();                        // consume "+"
			to_be_returned = make_unique<OpADD>(move(to_be_returned), parse_rval_multiplication());
			break;

		case MINUS:
			getNextToken();                        // consume "-"
			to_be_returned = make_unique<OpSUB>(move(to_be_returned), parse_rval_multiplication());
			break;

		default:
			loop = false;
		}
	}
	return to_be_returned;
}


static auto parse_rval_inequality()
{
	auto to_be_returned = parse_rval_addition();
	bool loop           = true;

	while (loop)
	{
		switch (CurTok.type)
		{
		case LT:
			getNextToken();                        // consume "<"
			to_be_returned = make_unique<OpLT>(move(to_be_returned), parse_rval_addition());
			break;

		case LE:
			getNextToken();                        // consume "<="
			to_be_returned = make_unique<OpLE>(move(to_be_returned), parse_rval_addition());
			break;

		case GE:
			getNextToken();                        // consume ">="
			to_be_returned = make_unique<OpGE>(move(to_be_returned), parse_rval_addition());
			break;

		case GT:
			getNextToken();                        // consume ">"
			to_be_returned = make_unique<OpGT>(move(to_be_returned), parse_rval_addition());
			break;

		default:
			loop = false;
		}
	}
	// cout << "No more rval ineqality " << CurTok.type << '\n';
	return to_be_returned;
}


static auto parse_rval_equality()
{
	auto to_be_returned = parse_rval_inequality();
	bool loop           = true;

	while (loop)
	{
		switch (CurTok.type)
		{
		case EQ:
			getNextToken();                        // consume "=="
			to_be_returned = make_unique<OpEQ>(move(to_be_returned), parse_rval_inequality());
			break;

		case NE:
			getNextToken();                        // consume "!="
			to_be_returned = make_unique<OpNE>(move(to_be_returned), parse_rval_inequality());
			break;

		default:
			loop = false;
		}
	}
	// cout << "No more rval equality " << CurTok.type << '\n';
	return to_be_returned;
}


static unique_ptr<ASTnode> parse_rval_conjunction()
{
	auto equality = parse_rval_equality();

	if (CurTok.type == AND)
	{
		auto to_be_returned = make_unique<ConjunctionNode>();
		to_be_returned->addSub(move(equality));
		while (CurTok.type == AND)
		{
			getNextToken();                // consume "&&"
			to_be_returned->addSub(parse_rval_equality());
		}
		return to_be_returned;
	}
	return equality;
	// cout << "No more rval conjunctions " << CurTok.type << '\n';
}


static unique_ptr<ASTnode> parse_rval()
{
	auto con = parse_rval_conjunction();

	if (CurTok.type == OR)
	{
		auto to_be_returned = make_unique<DisjunctionNode>();
		to_be_returned->addSub(move(con));
		while (CurTok.type == OR)
		{
			getNextToken();                // consume "||"
			to_be_returned->addSub(parse_rval_conjunction());
		}
		return to_be_returned;
	}

	// cout << "No more rval disunctions " << CurTok.type << '\n';
	return con;
}


static unique_ptr<ParamNode> parse_param()
{
	int param_type = 0;

	switch (CurTok.type)
	{
	case INT_TOK:
		param_type = INT_TYPE;
		break;

	case FLOAT_TOK:
		param_type = FLOAT_TYPE;
		break;

	case BOOL_TOK:
		param_type = BOOL_TYPE;
		break;

	default:
		throw syntax_error("Invalid param type");
	}
	getNextToken();      // Consume type token
	assert_tok(IDENT, "Identifier must follow type in parameter list");
	auto r = make_unique<ParamNode>(CurTok.lexeme, param_type);

	getNextToken();      // Consume IDENT
	return r;
}


static unique_ptr<ASTnode> parse_params()
{
	auto params = make_unique<ParamsNode>();

	if (CurTok.type == VOID_TOK)
	{
		getNextToken();           // Consume "void"
		assert_tok(RPAR, "Expected closing parenthesis after void keyword in parameter list");
		return params;
	}
	if (CurTok.type == RPAR)
	{
		return params;
	}
	params->addSub(parse_param());
	while (CurTok.type != RPAR)
	{
		assert_tok(COMMA, "Expected comma after parameter in parameter list");
		getNextToken();           // Consume ','
		params->addSub(parse_param());
	}
	return params;
}


static unique_ptr<ASTnode> parse_extern()
{
	assert_tok(EXTERN, "Expected 'extern' keyword");
	getNextToken();      // Consume "extern"
	char return_type;

	switch (CurTok.type)
	{
	case INT_TOK:
		return_type = INT_TYPE;
		break;

	case FLOAT_TOK:
		return_type = FLOAT_TYPE;
		break;

	case BOOL_TOK:
		return_type = BOOL_TYPE;
		break;

	case VOID_TOK:
		return_type = VOID_TYPE;
		break;

	default:
		throw syntax_error("Type must follow extern keyword");
	}
	assert(CurTok.type == INT_TOK || CurTok.type == FLOAT_TOK || CurTok.type == BOOL_TOK || CurTok.type == VOID_TOK);
	getNextToken();      // Consume type token
	assert_tok(IDENT, "Expected function name directly after extern keyword");
	auto fun_name = CurTok.lexeme;

	if (ExternedFunctions.count(fun_name))
	{
		throw semantic_error("Attempted to redefine extern function " + fun_name);
	}
	getNextToken();      // Consume IDENT
	assert_tok(LPAR, "Expected opening left parenthesis '(' after function name");
	getNextToken();      // Consume "("
	auto fun_params = parse_params();

	assert_tok(RPAR, "Expected closing right parenthesis ')' after parameter list");
	getNextToken();      // Consume ")"
	assert_tok(SC, "Expected semicolon after extern function signature");
	getNextToken();      // Consume ";"
	auto tbr = make_unique<FunctionSignature>(fun_name, return_type, move(fun_params));

	return tbr;
}


static auto parse_extern_list()
{
	auto extern_list = make_unique<ExternListNode>();

	while (CurTok.type == EXTERN)
	{
		extern_list->addSub(parse_extern());
	}
	return extern_list;
}


static unique_ptr<ASTnode> parse_local_decls()
{
	auto local_decls = make_unique<LocalDeclsNode>();
	bool loop        = true;

	while (loop)
	{
		char var_type;
		switch (CurTok.type)
		{
		case INT_TOK:
			var_type = INT_TYPE;
			break;

		case FLOAT_TOK:
			var_type = FLOAT_TYPE;
			break;

		case BOOL_TOK:
			var_type = BOOL_TYPE;
			break;

		default:
			loop = false;
		}
		if (loop)
		{
			getNextToken();                // Consume type tok
			assert_tok(IDENT, "Expected identifier after type in local declaration");

			auto var_name = CurTok.lexeme;
			auto decl     = make_unique<VarDeclNode>(ActiveScopes.top(), var_name, var_type, false);
			ActiveScopes.top()->setDecl(var_name, decl.get());
			local_decls->addSub(move(decl));

			getNextToken();                // Consume IDENT
			assert_tok(SC, "Expected semicolon ';' at the end of local declaration");
			getNextToken();                // Consume ';'
		}
	}
	return local_decls;
}


static unique_ptr<ASTnode> parse_expr_stmt()
{
	// todo check current token
	if (CurTok.type == SC)
	{
		getNextToken();           // Consume ';'
		return make_unique<NullStmt>();
	}

	auto expr_stmt = parse_expr();

	assert_tok(SC, "Expected semicolon ';' at the end of expression");
	getNextToken();      // Consume ';'
	return expr_stmt;
}


static unique_ptr<ASTnode> parse_block()
{
	auto new_block = make_unique<BlockNode>();

	ActiveScopes.push(new_block->scope);

	assert_tok(LBRA, "Expected opening left brace '{' at the start of block");
	getNextToken();      // Consume "{"
	auto decls = parse_local_decls();
	auto stmts = parse_stmt_list();

	assert_tok(RBRA, "Expected closing right brace '}' at the start of block");
	getNextToken();      // Consume "}"
	new_block->addSub(move(stmts));
	new_block->addSub(move(decls));
	ActiveScopes.pop();
	return new_block;
}


static unique_ptr<ASTnode> parse_if_stmt()
{
	assert_tok(IF, "Expected if at start of if-statement. This should not happen :(");
	getNextToken();      // Consume "if"
	assert_tok(LPAR, "Expected opening left parenthesis '(' after if keyword");
	getNextToken();      // Consume '('
	auto if_cond = parse_expr();

	assert_tok(RPAR, "Expected closing right parenthesis ')' after if condition");
	getNextToken();      // Consume ')'
	auto if_body = parse_block();

	if (CurTok.type == ELSE)
	{
		getNextToken();           // Consume "else"
		auto else_body = parse_block();
		return make_unique<IfWithElseNode>(move(if_cond), move(if_body), move(else_body));
	}
	else
	{
		return make_unique<IfStmt>(move(if_cond), move(if_body));
	}
}

static unique_ptr<ASTnode> parse_while_stmt()
{
	assert_tok(WHILE, "Expected while keyword at the start of while loop. This should not happen :(");
	getNextToken();      // Consume "while"
	assert_tok(LPAR, "Expected opening left parenthesis '(' after while keyword");
	getNextToken();      // Consume '('
	auto condition = parse_expr();

	assert_tok(RPAR, "Expected closing right parenthesis ')' after while condition");
	getNextToken();      // Consume ')'
	auto loop_body = parse_stmt();

	return make_unique<WhileStmt>(move(condition), move(loop_body));
}


static unique_ptr<ASTnode> parse_return_stmt()
{
	assert_tok(RETURN, "Expected return keyword at the start of return statement. This should not happen :{");
	getNextToken();        // Consume "return"
	if (CurTok.type == SC) // Return nothing
	{
		getNextToken();    // Consume ';'
		return make_unique<ReturnNothingNode>();
	}
	else      // Return expr
	{
		auto stmt = make_unique<ReturnValueNode>(parse_expr());
		assert_tok(SC, "Expected semicolon ';' after return statement");
		getNextToken();           // Consume ';'
		return stmt;
	}
}


static unique_ptr<ASTnode> parse_stmt()
{
	switch (CurTok.type)
	{
	case LBRA:
		return parse_block();

	case IF:
		return parse_if_stmt();

	case WHILE:
		return parse_while_stmt();

	case RETURN:
		return parse_return_stmt();

	default:
		return parse_expr_stmt();
	}
}


static unique_ptr<ASTnode> parse_stmt_list()
{
	auto stmt_list = make_unique<StmtListNode>();

	while (CurTok.type != RBRA)
	{
		stmt_list->addSub(parse_stmt());
	}
	return stmt_list;
}


// Parses global variable declarations and function definitions
static unique_ptr<ASTnode> parse_decl()
{
	assert_tok_any({ INT_TOK, FLOAT_TOK, BOOL_TOK, VOID_TOK }, "Expected type keyword at the start of declaration");
	if (CurTok.type == VOID_TOK)
	{
		auto fun_type = VOID_TYPE;
		// Parse a void function
		getNextToken();           // Consume "void"
		assert_tok(IDENT, "Expected identifier after void keyword");
		auto fun_name = CurTok.lexeme;
		if (ActiveScopes.top()->getDecl(fun_name))
		{
			throw semantic_error("Name " + fun_name + " has already been declared a variable");
		}
		if (ExternedFunctions.count(fun_name))
		{
			throw semantic_error("Function " + fun_name + " has already been defined as extern previously");
		}
		if (DefinedFunctions.count(fun_name))
		{
			throw semantic_error("Function " + fun_name + " has already been defined previously");
		}
		getNextToken();           // Consume IDENT
		assert_tok(LPAR, "Expected opening left parenthesis '(' after identifier");
		getNextToken();           // Consume '('
		auto fun_params = parse_params();
		assert_tok(RPAR, "Expected closing right parenthesis ')' after parameter list");
		getNextToken();           // Consume ')'
		auto fun_sig  = make_unique<FunctionSignature>(fun_name, fun_type, move(fun_params));
		auto fun_body = parse_block();
		auto fun_def  = make_unique<FunDeclNode>(move(fun_sig), move(fun_body));
		DefinedFunctions[fun_name] = fun_def.get();
		return fun_def;
	}
	char decl_type;

	switch (CurTok.type)
	{
	case INT_TOK:
		decl_type = INT_TYPE;
		break;

	case FLOAT_TOK:
		decl_type = FLOAT_TYPE;
		break;

	case BOOL_TOK:
		decl_type = BOOL_TYPE;
		// break;

		// default:
		// 	throw compiler_error();
	}
	getNextToken();      // Consume type token
	auto decl_name = CurTok.lexeme;

	if (static_cast<bool>(ActiveScopes.top()->getDecl(decl_name)))
	{
		throw semantic_error("Identifier '" + decl_name + "' has been used before for variable declaration");
	}
	if (ExternedFunctions.count(decl_name))
	{
		throw semantic_error("Identifier '" + decl_name + "' has been used before as extern");
	}
	if (DefinedFunctions.count(decl_name))
	{
		throw semantic_error("Identifier '" + decl_name + "' has been used before as function definition");
	}

	getNextToken();      // Consume IDENT
	if (CurTok.type == SC)
	{
		// Parse variable decl
		getNextToken();           // Consume ';'
		auto var_decl = make_unique<VarDeclNode>(ActiveScopes.top(), decl_name, decl_type, true);
		ActiveScopes.top()->setDecl(decl_name, var_decl.get());
		return move(var_decl);
	}
	else if (CurTok.type == LPAR)
	{
		// Parse function decl
		getNextToken();           // Consume '('
		auto params = parse_params();
		assert_tok(RPAR, "Expected closing right parenthesis ')' after parameter list");
		getNextToken();            // Consume ')'

		auto sig     = make_unique<FunctionSignature>(decl_name, decl_type, move(params));
		auto body    = parse_block();
		auto fun_def = make_unique<FunDeclNode>(move(sig), move(body));
		DefinedFunctions[decl_name] = fun_def.get();
		return fun_def;
	}
	else
	{
		throw syntax_error("Expected either semicolon or opening left parenthesis after identifier");
	}
}


static auto parse_decl_list()
{
	auto decl_list = make_unique<DeclListNode>();

	while (CurTok.type != EOF_TOK)
	{
		decl_list->addSub(parse_decl());
	}
	assert(decl_list->children.size() > 0);
	// TODO error if no main
	return decl_list;
}


// program ::= extern_list decl_list | decl_list
static unique_ptr<ProgramNode> parse_program()
{
	return make_unique<ProgramNode>(parse_extern_list(), parse_decl_list());
}


static unique_ptr<ProgramNode> parser()
{
	return parse_program();
}


//===----------------------------------------------------------------------===//
// AST Printer
//===----------------------------------------------------------------------===//

inline llvm::raw_ostream& operator<<(llvm::raw_ostream& os, const ASTnode& ast)
{
	os << ast.to_string();
	return os;
}


static void print_ast(ASTnode *root, int level = 0)
{
	string pre = "";

	for (int i = 0; i < level; ++i)
	{
		pre += '\t';
	}
	cout << pre << root->to_string() << '\n';
	if (root->children.size())
	{
		for (int i = 0; i < root->children.size(); ++i)
		{
			print_ast(root->children[i].get(), level + 1);
		}
	}
}


//===----------------------------------------------------------------------===//
// Code Generation
//===----------------------------------------------------------------------===//

// Todo whilestmnt paramnode codegen

// todo handle empty function body when codegening




//===----------------------------------------------------------------------===//
// Main driver code.
//===----------------------------------------------------------------------===//

int main(int argc, char **argv)
{
	auto globalScope = VariableScope();

	ActiveScopes.push(&globalScope);
	if (argc == 2)
	{
		pFile = fopen(argv[1], "r");
		if (pFile == NULL)
		{
			perror("Error opening file");
		}
	}
	else
	{
		cout << "Usage: ./code InputFile\n";
		return 1;
	}

	// initialize line number and column numbers to zero
	lineNo   = 1;
	columnNo = 1;

	// get the first token
	getNextToken();
	// while (CurTok.type != EOF_TOK) {
	//    fprintf(stderr, "Token: %s with type %d\n", CurTok.lexeme.c_str(),
	//                CurTok.type);
	//    getNextToken();
	// }
	cerr << "Lexer Finished\n";

	// Run the parser now.
	try
	{
		auto program = parser();
		cerr << "Parsing Finished\n";
		print_ast(program.get());
		program->codegen();
	}
	catch (syntax_error& e)
	{
		cerr << "Parsing ERROR on line " << e.lineNo << " column " << columnNo << " received token " << e.erroneous_token << endl;
		cerr << e.what() << '\n';
	}
	catch (semantic_error& e)
	{
		cerr << "Semantic ERROR on line " << e.lineNo << " column " << columnNo << " received token " << e.erroneous_token << endl;
		cerr << e.what() << '\n';
	}
	catch (compiler_error& e)
	{
		cerr << "Compiler ERROR on line " << e.lineNo << " column " << columnNo << " received token " << e.erroneous_token << endl;
		cerr << e.what() << '\n';
	}

	//********************* Start printing final IR **************************
	// Print out all of the generated code into a file called output.ll
	auto            Filename = "output.ll";
	std::error_code EC;
	raw_fd_ostream  dest(Filename, EC, sys::fs::OF_None);

	if (EC)
	{
		errs() << "Could not open file: " << EC.message();
		return 1;
	}
	// TheModule->print(errs(), nullptr); // print IR to terminal
	TheModule->print(dest, nullptr);
	//********************* End printing final IR ****************************

	fclose(pFile);      // close the file that contains the code that was parsed
	return 0;
}
