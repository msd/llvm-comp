#include "llvm/ADT/APFloat.h"
#include "llvm/ADT/Optional.h"
#include "llvm/ADT/STLExtras.h"
#include "llvm/IR/BasicBlock.h"
#include "llvm/IR/Constants.h"
#include "llvm/IR/DerivedTypes.h"
#include "llvm/IR/Function.h"
#include "llvm/IR/GlobalVariable.h"
#include "llvm/IR/IRBuilder.h"
#include "llvm/IR/Instructions.h"
#include "llvm/IR/LLVMContext.h"
#include "llvm/IR/LegacyPassManager.h"
#include "llvm/IR/Module.h"
#include "llvm/IR/Type.h"
#include "llvm/IR/Verifier.h"
#include "llvm/Support/FileSystem.h"
#include "llvm/Support/Host.h"
#include "llvm/Support/TargetRegistry.h"
#include "llvm/Support/TargetSelect.h"
#include "llvm/Support/raw_ostream.h"
#include "llvm/Target/TargetMachine.h"
#include "llvm/Target/TargetOptions.h"
#include <algorithm>
#include <cassert>
#include <cctype>
#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <map>
#include <memory>
#include <queue>
#include <string.h>
#include <string>
#include <system_error>
#include <utility>
#include <vector>
#include <stdexcept>
#include <stack>

using namespace llvm;
using namespace llvm::sys;

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

// TOKEN struct is used to keep track of information about a token
struct TOKEN
{
	int         type = -100;
	std::string lexeme;
	int         lineNo;
	int         columnNo;
};

static std::string IdentifierStr; // Filled in if IDENT
static int         IntVal;        // Filled in if INT_LIT
static bool        BoolVal;       // Filled in if BOOL_LIT
static float       FloatVal;      // Filled in if FLOAT_LIT
static std::string StringVal;     // Filled in if String Literal
static int         lineNo, columnNo;

static TOKEN returnTok(std::string lexVal, int tok_type)
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
		std::string NumStr;

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
	std::string s(1, ThisChar);

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
static TOKEN             CurTok;
static std::deque<TOKEN> tok_buffer;

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

const char INVALID_TYPE = 0;
const char INT_TYPE     = 3;
const char BOOL_TYPE    = 2;
const char FLOAT_TYPE   = 1;
const char VOID_TYPE    = 4;

static std::string type_to_str(const char type);

/// ASTnode - Base class for all AST nodes.
class ASTnode
{
public:
	std::vector<std::unique_ptr<ASTnode> > children{};
	virtual Value *codegen() = 0;
	virtual const std::string node_type() const = 0;

	virtual std::string to_string() const
	{
		return node_type() + " node with " + std::to_string(children.size()) + " child(ren)";
	}

	virtual ~ASTnode() {}
	virtual void addSub(std::unique_ptr<ASTnode> subExpr)
	{
		children.push_back(std::move(subExpr));
	}

	void setSubs(std::vector<std::unique_ptr<ASTnode> > subs)
	{
		children = std::move(subs);
	}

	ASTnode *getSub(int i)
	{
		return children[i].get();
	}
};

class ExternListNode : public ASTnode
{
public:
	virtual const std::string node_type() const
	{
		return "EXTERN LIST";
	}

	virtual Value *codegen();
};


class NullStmt : public ASTnode
{
public:
	virtual const std::string node_type() const
	{
		return "NULL STMT";
	}

	virtual Value *codegen();
};


class DeclNode : public ASTnode
{
	virtual const std::string node_type() const = 0;
};

class VariableScope;

class VarDeclNode : public DeclNode
{
public:
	VariableScope *scope;
	const std::string var_name;
	const char var_type;
	const bool is_global;
	VarDeclNode(VariableScope *scope, std::string var_name, const char var_type, bool is_global) : scope(scope), var_name(var_name), var_type(var_type), is_global(is_global) {}

	virtual const std::string node_type() const
	{
		return "VAR DECL";
	}

	virtual std::string to_string() const
	{
		return node_type() + " node NAME=" + var_name + " TYPE=" + type_to_str(var_type);
	}

	virtual Value *codegen();
};

class semantic_error;

class VariableScope
{
	VariableScope *inherited;
	// std::map<std::string, Value *> local_decls{};
	std::map<std::string, Value *> local_addrs{};
	std::map<std::string, VarDeclNode *> local_decls{};
	VariableScope(VariableScope *inherited) : inherited(inherited) {}
public:
	VariableScope() : inherited(nullptr) {}
	static auto inherit_vars(VariableScope *outer_scope)
	{
		auto new_scope = new VariableScope(outer_scope);

		return std::unique_ptr<VariableScope>{ new_scope };
	}

	auto hasLocalName(std::string var_name)
	{
		return static_cast<bool>(local_decls.count(var_name));
	}

	bool hasName(std::string var_name)
	{
		if (!(hasLocalName(var_name) || (inherited && inherited->hasName(var_name))))
		{
			std::cout << "DECL LOOKUP FAILED: " << var_name << std::endl;
		}
		return hasLocalName(var_name) || (inherited && inherited->hasName(var_name));
	}

	void setAddr(std::string var_name, Value *addr);

	void setDecl(std::string var_name, VarDeclNode *decl);

	// todo check calls to getDecl can be substituted for hasLocalName
	VarDeclNode *getDecl(std::string identifier)
	{
		if (local_decls.count(identifier))
		{
			return local_decls[identifier];
		}
		if (inherited)
		{
			return inherited->getDecl(identifier);
		}
		return nullptr;
	}

	bool hasAddr(std::string var_name)
	{
		if (hasLocalName(var_name))
		{
			return local_addrs.count(var_name);
		}
		if (inherited)
		{
			return inherited->hasAddr(var_name);
		}
		return false;
	}

	Value *getAddr(std::string var_name);
};

class BlockNode : public ASTnode
{
private:
	std::unique_ptr<VariableScope> _scope;
public:
	VariableScope *scope;

	ASTnode *local_decls()
	{
		return children[0].get();
	}

	ASTnode *stmt_list()
	{
		return children[1].get();
	}

	virtual const std::string node_type() const
	{
		return "BLOCK";
	}

	BlockNode();
	virtual Value *codegen();
};

class ExprNode : public ASTnode
{
public:
	virtual char expr_type() = 0;
	virtual Value *codegen() = 0;
};

class IfStmt : public ASTnode
{
public:
	ExprNode *cond;
	BlockNode *then_body;
	IfStmt(std::unique_ptr<ASTnode> if_cond, std::unique_ptr<ASTnode> if_body)
	{
		addSub(std::move(if_cond));
		addSub(std::move(if_body));
		cond      = dynamic_cast<ExprNode *>(children[0].get());
		then_body = dynamic_cast<BlockNode *>(children[1].get());
	}

	virtual const std::string node_type() const
	{
		return "IF STMT";
	}

	virtual Value *codegen();
};

class IfWithElseNode : public ASTnode
{
public:
	ExprNode *cond;
	BlockNode *then_body, *else_body;
	IfWithElseNode(std::unique_ptr<ASTnode> if_cond, std::unique_ptr<ASTnode> then_body, std::unique_ptr<ASTnode> else_body)
	{
		addSub(std::move(if_cond));
		addSub(std::move(then_body));
		addSub(std::move(else_body));
		cond            = dynamic_cast<ExprNode *>(children[0].get());
		this->then_body = dynamic_cast<BlockNode *>(children[1].get());
		this->else_body = dynamic_cast<BlockNode *>(children[2].get());
	}

	virtual const std::string node_type() const
	{
		return "IF WITH ELSE STMT";
	}

	virtual Value *codegen();
};

class RvalNode : public ExprNode
{
public:
	virtual ~RvalNode() {}
	virtual char expr_type() = 0;
	virtual const std::string node_type() const = 0;
	virtual Value *codegen() = 0;
};


class ParenExprNode : public RvalNode
{
public:
	ParenExprNode(std::unique_ptr<ASTnode> expr)
	{
		children.push_back(std::move(expr));
	}

	RvalNode *expr()
	{
		return dynamic_cast<RvalNode *>(children[0].get());
	}

	virtual const std::string node_type() const
	{
		return "PAREN EXPR";
	}

	virtual char expr_type()
	{
		return expr()->expr_type();
	}

	virtual Value *codegen();
};


class WhileStmt : public ASTnode
{
public:
	WhileStmt(std::unique_ptr<ASTnode> condition, std::unique_ptr<ASTnode> loop_body)
	{
		children.push_back(std::move(condition));
		children.push_back(std::move(loop_body));
	}

	ASTnode *condition()
	{
		return children[0].get();
	}

	ASTnode *loop_body()
	{
		return children[1].get();
	}

	virtual const std::string node_type() const
	{
		return "WHILE STMT";
	}

	virtual Value *codegen();
};

template<typename T>
class LitNode : public RvalNode
{
public:
	T *data;
	LitNode(T litVal)
	{
		data  = new T;
		*data = litVal;
	}

	virtual std::string data_str() const
	{
		return std::to_string(*data);
	}

	virtual std::string to_string() const
	{
		return node_type() + "=" + data_str();
	}

	~LitNode()
	{
		delete data;
	}

	virtual char expr_type() = 0;
};

/// IntASTnode - Class for integer literals like 1, 2, 10,
class IntNode : public LitNode<int>
{
public:
	IntNode(int litVal) : LitNode<int>(litVal) {}
	virtual const std::string node_type() const
	{
		return "INT_LIT";
	}

	int get_int()
	{
		return *static_cast<int *>(data);
	}

	virtual Value *codegen();

	virtual char expr_type()
	{
		return INT_TYPE;
	}

	float as_float()
	{
		return static_cast<float>(get_int());
	}
};

class BoolNode : public LitNode<bool>
{
public:
	bool get_bool()
	{
		return *static_cast<bool *>(data);
	}

	virtual const std::string node_type() const
	{
		return "BOOL_LIT";
	}

	virtual Value *codegen();

	BoolNode(bool litVal) : LitNode<bool>(litVal) {}
	virtual char expr_type()
	{
		return BOOL_TYPE;
	}

	int as_int()
	{
		return get_bool()? 1: 0;
	}

	float as_float()
	{
		return static_cast<float>(get_bool());
	}
};

class FloatNode : public LitNode<float>
{
public:
	FloatNode(float litVal) : LitNode<float>(litVal) {}
	virtual std::string to_string() const
	{
		return node_type() + "=" + std::to_string(*static_cast<float *>(data));
	}

	virtual const std::string node_type() const
	{
		return "FLOAT_LIT";
	}

	float get_float()
	{
		return *static_cast<float *>(data);
	}

	virtual Value *codegen();

	virtual char expr_type()
	{
		return FLOAT_TYPE;
	}
};

class LocalDeclsNode : public ASTnode
{
public:
	virtual const std::string node_type() const
	{
		return "LOCAL DECLS";
	}

	virtual Value *codegen();
};

class StmtListNode : public ASTnode
{
public:
	virtual const std::string node_type() const
	{
		return "STMTS";
	}

	virtual Value *codegen();
};

class NegationNode : public RvalNode
{
public:
	NegationNode(std::unique_ptr<ASTnode> rval)
	{
		addSub(std::move(rval));
	}

	RvalNode *negatee()
	{
		return dynamic_cast<RvalNode *>(children[0].get());
	}

	virtual const std::string node_type() const
	{
		return "NEG";
	}

	virtual char expr_type()
	{
		auto negatee_type = negatee()->expr_type();

		if (negatee_type == BOOL_TYPE)
		{
			return INT_TYPE;
		}
		return negatee_type;
	}

	virtual Value *codegen();
};


class FunCallNode : public RvalNode
{
public:
	const std::string fun_name;
	FunCallNode(std::string fun_name) : fun_name(fun_name) {}
	virtual const std::string node_type() const
	{
		return "FUNCTION CALL";
	}

	virtual std::string to_string() const
	{
		return node_type() + " node CALLEE=" + fun_name;
	}

	virtual ~FunCallNode() {}

	virtual char expr_type();
	virtual Value *codegen();
};

class NotNode : public RvalNode
{
public:
	NotNode(std::unique_ptr<ASTnode> sub)
	{
		addSub(std::move(sub));
	}

	RvalNode *negatee()
	{
		return dynamic_cast<RvalNode *>(children[0].get());
	}

	virtual const std::string node_type() const
	{
		return "NOT";
	}

	virtual char expr_type()
	{
		return BOOL_TYPE;
	}

	virtual Value *codegen();
};

class OperatorNode : public RvalNode
{
public:
	OperatorNode(std::unique_ptr<ASTnode> lhs, std::unique_ptr<ASTnode> rhs)
	{
		children.push_back(std::move(lhs));
		children.push_back(std::move(rhs));
	}

	RvalNode *lhs()
	{
		return dynamic_cast<RvalNode *>(children[0].get());
	}

	RvalNode *rhs()
	{
		return dynamic_cast<RvalNode *>(children[1].get());
	}

	virtual std::string to_string() const
	{
		return node_type() + " node with 2 children";
	}
};

class DisCon : public RvalNode
{
public:
	std::vector<std::unique_ptr<ASTnode> > clauses;
	void addClause(std::unique_ptr<ASTnode> clause)
	{
		clauses.push_back(std::move(clause));
	}

	virtual char expr_type()
	{
		return BOOL_TYPE;
	}
};

class ConjunctionNode : public DisCon
{
public:
	virtual const std::string node_type() const
	{
		return "CONJUNCTION";
	}

	virtual Value *codegen();
};

class DisjunctionNode : public DisCon
{
public:
	virtual const std::string node_type() const
	{
		return "DISJUNCTION";
	}

	virtual Value *codegen();
};

class OpADD : public OperatorNode
{
public:
	OpADD(std::unique_ptr<ASTnode> lhs, std::unique_ptr<ASTnode> rhs)
		: OperatorNode(std::move(lhs), std::move(rhs)) {}
	virtual const std::string node_type() const
	{
		return "ADDITION OPERATOR";
	}

	virtual char expr_type()
	{
		auto l_type = lhs()->expr_type(), r_type = rhs()->expr_type();

		if ((l_type == FLOAT_TYPE) || (r_type == FLOAT_TYPE))
		{
			return FLOAT_TYPE;
		}
		return INT_TYPE;
	}

	virtual Value *codegen();
};

class OpMULT : public OperatorNode
{
public:
	OpMULT(std::unique_ptr<ASTnode> lhs, std::unique_ptr<ASTnode> rhs)
		: OperatorNode(std::move(lhs), std::move(rhs)) {}
	virtual const std::string node_type() const
	{
		return "MULTIPLICATION OPERATOR";
	}

	virtual char expr_type()
	{
		auto l_type = lhs()->expr_type(), r_type = rhs()->expr_type();

		if ((l_type == FLOAT_TYPE) || (r_type == FLOAT_TYPE))
		{
			return FLOAT_TYPE;
		}
		return INT_TYPE;
	}

	virtual Value *codegen();
};

class OpDIV : public OperatorNode
{
public:
	OpDIV(std::unique_ptr<ASTnode> lhs, std::unique_ptr<ASTnode> rhs)
		: OperatorNode(std::move(lhs), std::move(rhs)) {}
	virtual const std::string node_type() const
	{
		return "DIVISION OPERATOR";
	}

	virtual char expr_type()
	{
		auto l_type = lhs()->expr_type(), r_type = rhs()->expr_type();

		if ((l_type == FLOAT_TYPE) || (r_type == FLOAT_TYPE))
		{
			return FLOAT_TYPE;
		}
		return INT_TYPE;
	}

	virtual Value *codegen();
};

class OpMODULO : public OperatorNode
{
public:
	OpMODULO(std::unique_ptr<ASTnode> lhs, std::unique_ptr<ASTnode> rhs)
		: OperatorNode(std::move(lhs), std::move(rhs)) {}
	virtual const std::string node_type() const
	{
		return "MODULO OPERATOR";
	}

	virtual char expr_type()
	{
		return INT_TYPE;
	}

	virtual Value *codegen();
};

class OpSUB : public OperatorNode
{
public:
	OpSUB(std::unique_ptr<ASTnode> lhs, std::unique_ptr<ASTnode> rhs)
		: OperatorNode(std::move(lhs), std::move(rhs)) {}
	virtual const std::string node_type() const
	{
		return "SUBTRACTION OPERATOR";
	}

	virtual char expr_type()
	{
		auto l_type = lhs()->expr_type(), r_type = rhs()->expr_type();

		if ((l_type == FLOAT_TYPE) || (r_type == FLOAT_TYPE))
		{
			return FLOAT_TYPE;
		}
		return INT_TYPE;
	}

	virtual Value *codegen();
};

class EqualityNode : public OperatorNode
{
public:
	EqualityNode(std::unique_ptr<ASTnode> lhs, std::unique_ptr<ASTnode> rhs)
		: OperatorNode(std::move(lhs), std::move(rhs)) {}
	virtual const std::string node_type() const = 0;

	virtual char expr_type()
	{
		return BOOL_TYPE;
	}
};

class InequalityNode : public OperatorNode
{
public:
	InequalityNode(std::unique_ptr<ASTnode> lhs, std::unique_ptr<ASTnode> rhs)
		: OperatorNode(std::move(lhs), std::move(rhs)) {}
	virtual const std::string node_type() const = 0;

	virtual char expr_type()
	{
		return BOOL_TYPE;
	}
};

class OpEQ : public EqualityNode
{
public:
	OpEQ(std::unique_ptr<ASTnode> lhs, std::unique_ptr<ASTnode> rhs)
		: EqualityNode(std::move(lhs), std::move(rhs)) {}
	virtual const std::string node_type() const
	{
		return "EQUALS OPERATOR";
	}

	virtual Value *codegen();
};

class OpNE : public EqualityNode
{
public:
	OpNE(std::unique_ptr<ASTnode> lhs, std::unique_ptr<ASTnode> rhs)
		: EqualityNode(std::move(lhs), std::move(rhs)) {}
	virtual const std::string node_type() const
	{
		return "NOT EQUALS OPERATOR";
	}

	virtual Value *codegen();
};

class OpLE : public InequalityNode
{
public:
	OpLE(std::unique_ptr<ASTnode> lhs, std::unique_ptr<ASTnode> rhs)
		: InequalityNode(std::move(lhs), std::move(rhs)) {}
	virtual const std::string node_type() const
	{
		return "LESS OR EQUAL OPERATOR";
	}

	virtual Value *codegen();
};

class OpLT : public InequalityNode
{
public:
	OpLT(std::unique_ptr<ASTnode> lhs, std::unique_ptr<ASTnode> rhs)
		: InequalityNode(std::move(lhs), std::move(rhs)) {}
	virtual const std::string node_type() const
	{
		return "LESS THAN OPERATOR";
	}

	virtual Value *codegen();
};

class OpGE : public InequalityNode
{
public:
	OpGE(std::unique_ptr<ASTnode> lhs, std::unique_ptr<ASTnode> rhs)
		: InequalityNode(std::move(lhs), std::move(rhs)) {}
	virtual const std::string node_type() const
	{
		return "GREATER OR EQUAL OPERATOR";
	}

	virtual Value *codegen();
};

class OpGT : public InequalityNode
{
public:
	OpGT(std::unique_ptr<ASTnode> lhs, std::unique_ptr<ASTnode> rhs)
		: InequalityNode(std::move(lhs), std::move(rhs)) {}
	virtual const std::string node_type() const
	{
		return "GREATER THAN OPERATOR";
	}

	virtual Value *codegen();
};

class ParamNode : public ASTnode
{
public:
	const std::string name;
	const char type;
	ParamNode(std::string name, const char type) : name(name), type(type) {}
	virtual const std::string node_type() const
	{
		return "PARAM";
	}

	virtual std::string to_string() const
	{
		return node_type() + " node NAME=" + name + ", TYPE=" + type_to_str(type);
	}

	Type *llvm_type();
	virtual Value *codegen();
};

class ParamsNode : public ASTnode
{
public:
	ParamNode *get(int i)
	{
		return dynamic_cast<ParamNode *>(children[i].get());
	}

	int param_count()
	{
		return children.size();
	}

	virtual const std::string node_type() const
	{
		return "PARAMS";
	}

	virtual Value *codegen();
};

class FunctionSignature : public ASTnode
{
public:
	std::string name;
	const char return_type;
	FunctionSignature(std::string name, const char return_type, std::unique_ptr<ASTnode> params);
	ParamsNode *params()
	{
		return dynamic_cast<ParamsNode *>(children[0].get());
	}

	virtual const std::string node_type() const
	{
		return "FUNCTION SIGNATURE";
	}

	virtual std::string return_node_type()
	{
		return type_to_str(return_type);
	}

	virtual std::string to_string() const
	{
		return node_type() + " node RETURN=" + type_to_str(return_type);
	}

	virtual Function *codegen();
};

class VarExprNode : public RvalNode
{
public:
	const std::string name;
	const VarDeclNode *decl;
	VariableScope *scope;
	VarExprNode(VariableScope *scope, std::string name, VarDeclNode *decl) : name(name), decl(decl), scope(scope) {}
	virtual const std::string node_type() const
	{
		return "VAR EXPR";
	}

	virtual std::string to_string() const
	{
		return node_type() + " node NAME=" + name;
	}

	virtual char expr_type()
	{
		return decl->var_type;
	}

	virtual Value *codegen();
};


class FunDeclNode : public DeclNode
{
public:
	FunDeclNode(std::unique_ptr<ASTnode> sig, std::unique_ptr<ASTnode> body)
	{
		children.push_back(std::move(sig));
		children.push_back(std::move(body));
	}

	FunctionSignature *sig() const
	{
		return dynamic_cast<FunctionSignature *>(children[0].get());
	}

	BlockNode *body()
	{
		return dynamic_cast<BlockNode *>(children[1].get());
	}

	virtual const std::string node_type() const
	{
		return "FUN DECL";
	}

	virtual std::string to_string() const
	{
		return node_type() + " node NAME=" + sig()->name + " RETURN=" + type_to_str(sig()->return_type);
	}

	virtual Function *codegen();
};

class DeclListNode : public ASTnode
{
public:
	virtual const std::string node_type() const
	{
		return "DECL LIST";
	}

	virtual Value *codegen();
};

class ProgramNode : public ASTnode
{
public:
	ExternListNode *extern_list;
	DeclListNode *decl_list;
	ProgramNode(std::unique_ptr<ExternListNode> eln, std::unique_ptr<DeclListNode> dln)
	{
		children.push_back(std::move(eln));
		children.push_back(std::move(dln));
		extern_list = dynamic_cast<ExternListNode *>(children[0].get());
		decl_list   = dynamic_cast<DeclListNode *>(children[1].get());
	}

	virtual const std::string node_type() const
	{
		return "PROGRAM";
	}

	virtual Value *codegen();
};

class AssignmentLHS : public ASTnode
{
public:
	std::string var_to;
	AssignmentLHS(std::string var_to) : var_to(var_to) {}
	virtual const std::string node_type() const
	{
		return "ASSISGNENT LHS";
	}

	virtual std::string to_string() const
	{
		return node_type() + " node TARGET_VAR=" + var_to;
	}

	virtual Value *codegen();
};

class AssignmentExpr : public ExprNode
{
	VariableScope *scope;
public:
	AssignmentLHS *lhs()
	{
		return dynamic_cast<AssignmentLHS *>(children[0].get());
	}

	ExprNode *rhs()
	{
		return dynamic_cast<ExprNode *>(children[1].get());
	}

	virtual const std::string node_type() const
	{
		return "ASSIGNMENT STMT";
	}

	virtual char expr_type()
	{
		return rhs()->expr_type();
	}

	AssignmentExpr(VariableScope *scope, std::unique_ptr<ASTnode> lhs, std::unique_ptr<ASTnode> rhs);
	virtual Value *codegen();
};

class ReturnNothingNode : public ASTnode
{
public:
	virtual const std::string node_type() const
	{
		return "RETURN NOTHING STMT";
	}

	virtual Value *codegen();
};

class ReturnValueNode : public ASTnode
{
public:
	ExprNode *expr;
	ReturnValueNode(std::unique_ptr<ASTnode> expr)
	{
		children.push_back(std::move(expr));

		this->expr = dynamic_cast<ExprNode *>(children[0].get());
	}

	virtual const std::string node_type() const
	{
		return "RETURN VALUE STMT";
	}

	virtual Value *codegen();
};

static LLVMContext             TheContext;
static IRBuilder<>             Builder(TheContext);
static std::unique_ptr<Module> TheModule;
static std::map<std::string, FunctionSignature *> ExternedFunctions;
static std::map<std::string, FunDeclNode *>       DefinedFunctions;
// Current scope is the last in the
static std::stack<VariableScope *> ActiveScopes;

static std::unique_ptr<ASTnode> parse_stmt();
static std::unique_ptr<ASTnode> parse_stmt_list();

//===----------------------------------------------------------------------===//
// Recursive Descent Parser - Function call for each production
//===----------------------------------------------------------------------===//

/* Add function calls for each production */

class semantic_error : std::exception
{
	std::string err_msg;
public:
	const int lineNo, columnNo;
	std::string erroneous_token;
	semantic_error(std::string err_msg) : err_msg(std::move(err_msg)), lineNo(CurTok.lineNo), columnNo(CurTok.columnNo), erroneous_token(CurTok.lexeme) {}
	semantic_error(std::string err_msg, int lineNo, int columnNo, std::string erroneous_token) : err_msg(std::move(err_msg)), lineNo(lineNo), columnNo(columnNo), erroneous_token(std::move(erroneous_token)) {}
	virtual const char *what() const throw()
	{
		return err_msg.c_str();
	}
};

class syntax_error : std::exception
{
	std::string err_msg;
public:
	const int lineNo, columnNo;
	std::string erroneous_token;
	syntax_error(std::string err_msg) : err_msg(std::move(err_msg)), lineNo(CurTok.lineNo), columnNo(CurTok.columnNo), erroneous_token(CurTok.lexeme) {}
	syntax_error(std::string err_msg, int lineNo, int columnNo, std::string erroneous_token) : err_msg(std::move(err_msg)), lineNo(lineNo), columnNo(columnNo), erroneous_token(std::move(erroneous_token)) {}
	virtual const char *what() const throw()
	{
		return err_msg.c_str();
	}
};

class compiler_error : std::exception
// These should only happen if there's a change in the language specification.
// They are not something that will be emitted but here just for completeness
// sake. Happens when a token that wasn't expected is received.
{
	std::string err_msg;
public:
	const int lineNo, columnNo;
	std::string erroneous_token;
	compiler_error() : err_msg("Unknown compiler error"), lineNo(CurTok.lineNo), columnNo(CurTok.columnNo), erroneous_token(CurTok.lexeme) {}
	compiler_error(std::string err_msg) : err_msg(std::move(err_msg)), lineNo(CurTok.lineNo), columnNo(CurTok.columnNo), erroneous_token(CurTok.lexeme) {}
	compiler_error(std::string err_msg, int lineNo, int columnNo, std::string erroneous_token) : err_msg(std::move(err_msg)), lineNo(lineNo), columnNo(columnNo), erroneous_token(std::move(erroneous_token)) {}
	virtual const char *what() const throw()
	{
		return err_msg.c_str();
	}
};

static std::unique_ptr<ASTnode> parse_rval();

static void assert_tok_any(std::vector<TOKEN_TYPE> tok_types, std::string err_msg)
{
	for (auto tok_type: tok_types)
	{
		if (CurTok.type == tok_type)
		{
			return;
		}
	}
	throw syntax_error(std::move(err_msg));
}


static void assert_tok(TOKEN_TYPE tok_type, std::string err_msg)
{
	if (CurTok.type != tok_type)
	{
		throw syntax_error(std::move(err_msg));
	}
}


static std::unique_ptr<ASTnode> LogError(std::string err_txt)
{
	std::cerr << "line " << CurTok.lineNo << " column "
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
	return std::make_unique<ParenExprNode>(std::move(r));
}


static auto parse_neg_term()
{
	assert_tok(MINUS, "Expected minus '-'");
	getNextToken();      // Consume minus '-'
	return std::make_unique<NegationNode>(parse_rval());
}


static std::unique_ptr<ASTnode> parse_expr()
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
			auto lhs = std::make_unique<AssignmentLHS>(ident_name);
			return std::make_unique<AssignmentExpr>(ActiveScopes.top(), std::move(lhs), parse_expr());
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
	auto args = std::vector<std::unique_ptr<ASTnode> >();

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
	auto r = std::make_unique<FloatNode>(FloatVal);

	getNextToken();      // Consume FLOAT_LIT
	return r;
}


static auto parse_rval_INT_LIT()
{
	assert_tok(INT_LIT, "Expected int literal");
	auto r = std::make_unique<IntNode>(IntVal);

	getNextToken();      // Consume INT_LIT
	return r;
}


static auto parse_rval_BOOL_LIT()
{
	assert_tok(BOOL_LIT, "Expected bool literal");
	auto r = std::make_unique<BoolNode>(BoolVal);

	getNextToken();      // Consume BOOL_LIT
	return r;
}


static std::unique_ptr<RvalNode> parse_rval_var_or_fun()
{
	assert_tok(IDENT, "Expected identifier");
	auto ident_name = CurTok.lexeme;

	getNextToken();          // Consume IDENT
	if (CurTok.type == LPAR) // Parse Function call
	{
		auto fun_call = std::make_unique<FunCallNode>(ident_name);
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
			return std::make_unique<VarExprNode>(ActiveScopes.top(), ident_name, var_decl);
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
	return std::make_unique<NotNode>(parse_rval());
}


static std::unique_ptr<ASTnode> parse_rval_term()
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
	auto to_be_returned = parse_rval_term();
	bool loop           = true;

	while (loop)
	{
		char l_type, r_type;
		std::unique_ptr<ASTnode> rhs;
		switch (CurTok.type)
		{
		case ASTERISK:
			getNextToken();                        // consume "*"
			to_be_returned = std::make_unique<OpMULT>(std::move(to_be_returned), parse_rval_term());
			break;

		case DIV:
			getNextToken();                        // consume "/"
			to_be_returned = std::make_unique<OpDIV>(std::move(to_be_returned), parse_rval_term());
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
			to_be_returned = std::make_unique<OpMODULO>(std::move(to_be_returned), parse_rval_term());
			break;

		default:
			loop = false;
		}
	}
	return to_be_returned;
}


static std::string type_to_str(const char type)
{
	switch (type)
	{
	case INT_TYPE:
		return "INT";

	case BOOL_TYPE:
		return "BOOL";

	case FLOAT_TYPE:
		return "FLOAT";

	case VOID_TYPE:
		return "VOID";
	}
	return "unknown type";
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
			to_be_returned = std::make_unique<OpADD>(std::move(to_be_returned), parse_rval_multiplication());
			break;

		case MINUS:
			getNextToken();                        // consume "-"
			to_be_returned = std::make_unique<OpSUB>(std::move(to_be_returned), parse_rval_multiplication());
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
			to_be_returned = std::make_unique<OpLT>(std::move(to_be_returned), parse_rval_addition());
			break;

		case LE:
			getNextToken();                        // consume "<="
			to_be_returned = std::make_unique<OpLE>(std::move(to_be_returned), parse_rval_addition());
			break;

		case GE:
			getNextToken();                        // consume ">="
			to_be_returned = std::make_unique<OpGE>(std::move(to_be_returned), parse_rval_addition());
			break;

		case GT:
			getNextToken();                        // consume ">"
			to_be_returned = std::make_unique<OpGT>(std::move(to_be_returned), parse_rval_addition());
			break;

		default:
			loop = false;
		}
	}
	// std::cout << "No more rval ineqality " << CurTok.type << '\n';
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
			to_be_returned = std::make_unique<OpEQ>(std::move(to_be_returned), parse_rval_inequality());
			break;

		case NE:
			getNextToken();                        // consume "!="
			to_be_returned = std::make_unique<OpNE>(std::move(to_be_returned), parse_rval_inequality());
			break;

		default:
			loop = false;
		}
	}
	// std::cout << "No more rval equality " << CurTok.type << '\n';
	return to_be_returned;
}


static std::unique_ptr<ASTnode> parse_rval_conjunction()
{
	auto equality = parse_rval_equality();

	if (CurTok.type == AND)
	{
		auto to_be_returned = std::make_unique<ConjunctionNode>();
		to_be_returned->addSub(std::move(equality));
		while (CurTok.type == AND)
		{
			getNextToken();                // consume "&&"
			to_be_returned->addSub(parse_rval_equality());
		}
		return to_be_returned;
	}
	return equality;
	// std::cout << "No more rval conjunctions " << CurTok.type << '\n';
}


static std::unique_ptr<ASTnode> parse_rval()
{
	auto con = parse_rval_conjunction();

	if (CurTok.type == OR)
	{
		auto to_be_returned = std::make_unique<DisjunctionNode>();
		to_be_returned->addSub(std::move(con));
		while (CurTok.type == OR)
		{
			getNextToken();                // consume "||"
			to_be_returned->addSub(parse_rval_conjunction());
		}
		return to_be_returned;
	}

	// std::cout << "No more rval disunctions " << CurTok.type << '\n';
	return con;
}


static std::unique_ptr<ParamNode> parse_param()
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
	auto r = std::make_unique<ParamNode>(CurTok.lexeme, param_type);

	getNextToken();      // Consume IDENT
	return r;
}


static std::unique_ptr<ASTnode> parse_params()
{
	auto params = std::make_unique<ParamsNode>();

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


static std::unique_ptr<ASTnode> parse_extern()
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
	auto tbr = std::make_unique<FunctionSignature>(fun_name, return_type, std::move(fun_params));

	return tbr;
}


static auto parse_extern_list()
{
	auto extern_list = std::make_unique<ExternListNode>();

	while (CurTok.type == EXTERN)
	{
		extern_list->addSub(parse_extern());
	}
	return extern_list;
}


static std::unique_ptr<ASTnode> parse_local_decls()
{
	auto local_decls = std::make_unique<LocalDeclsNode>();
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
			auto decl     = std::make_unique<VarDeclNode>(ActiveScopes.top(), var_name, var_type, false);
			ActiveScopes.top()->setDecl(var_name, decl.get());
			local_decls->addSub(std::move(decl));

			getNextToken();                // Consume IDENT
			assert_tok(SC, "Expected semicolon ';' at the end of local declaration");
			getNextToken();                // Consume ';'
		}
	}
	return local_decls;
}


static std::unique_ptr<ASTnode> parse_expr_stmt()
{
	// todo check current token
	if (CurTok.type == SC)
	{
		getNextToken();           // Consume ';'
		return std::make_unique<NullStmt>();
	}

	auto expr_stmt = parse_expr();

	assert_tok(SC, "Expected semicolon ';' at the end of expression");
	getNextToken();      // Consume ';'
	return expr_stmt;
}


static std::unique_ptr<ASTnode> parse_block()
{
	auto new_block = std::make_unique<BlockNode>();

	ActiveScopes.push(new_block->scope);

	assert_tok(LBRA, "Expected opening left brace '{' at the start of block");
	getNextToken();      // Consume "{"
	auto decls = parse_local_decls();
	auto stmts = parse_stmt_list();

	assert_tok(RBRA, "Expected closing right brace '}' at the start of block");
	getNextToken();      // Consume "}"
	new_block->addSub(std::move(stmts));
	new_block->addSub(std::move(decls));
	ActiveScopes.pop();
	return new_block;
}


static std::unique_ptr<ASTnode> parse_if_stmt()
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
		return std::make_unique<IfWithElseNode>(std::move(if_cond), std::move(if_body), std::move(else_body));
	}
	else
	{
		return std::make_unique<IfStmt>(std::move(if_cond), std::move(if_body));
	}
}


static std::unique_ptr<ASTnode> parse_while_stmt()
{
	assert_tok(WHILE, "Expected while keyword at the start of while loop. This should not happen :(");
	getNextToken();      // Consume "while"
	assert_tok(LPAR, "Expected opening left parenthesis '(' after while keyword");
	getNextToken();      // Consume '('
	auto condition = parse_expr();

	assert_tok(RPAR, "Expected closing right parenthesis ')' after while condition");
	getNextToken();      // Consume ')'
	auto loop_body = parse_stmt();

	return std::make_unique<WhileStmt>(std::move(condition), std::move(loop_body));
}


static std::unique_ptr<ASTnode> parse_return_stmt()
{
	assert_tok(RETURN, "Expected return keyword at the start of return statement. This should not happen :{");
	getNextToken();        // Consume "return"
	if (CurTok.type == SC) // Return nothing
	{
		getNextToken();    // Consume ';'
		return std::make_unique<ReturnNothingNode>();
	}
	else      // Return expr
	{
		auto stmt = std::make_unique<ReturnValueNode>(parse_expr());
		assert_tok(SC, "Expected semicolon ';' after return statement");
		getNextToken();           // Consume ';'
		return stmt;
	}
}


static std::unique_ptr<ASTnode> parse_stmt()
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


static std::unique_ptr<ASTnode> parse_stmt_list()
{
	auto stmt_list = std::make_unique<StmtListNode>();

	while (CurTok.type != RBRA)
	{
		stmt_list->addSub(parse_stmt());
	}
	return stmt_list;
}


// Parses global variable declarations and function definitions
static std::unique_ptr<ASTnode> parse_decl()
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
		auto fun_sig  = std::make_unique<FunctionSignature>(fun_name, fun_type, std::move(fun_params));
		auto fun_body = parse_block();
		auto fun_def  = std::make_unique<FunDeclNode>(std::move(fun_sig), std::move(fun_body));
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
		auto var_decl = std::make_unique<VarDeclNode>(ActiveScopes.top(), decl_name, decl_type, true);
		ActiveScopes.top()->setDecl(decl_name, var_decl.get());
		return std::move(var_decl);
	}
	else if (CurTok.type == LPAR)
	{
		// Parse function decl
		getNextToken();           // Consume '('
		auto params = parse_params();
		assert_tok(RPAR, "Expected closing right parenthesis ')' after parameter list");
		getNextToken();            // Consume ')'

		auto sig     = std::make_unique<FunctionSignature>(decl_name, decl_type, std::move(params));
		auto body    = parse_block();
		auto fun_def = std::make_unique<FunDeclNode>(std::move(sig), std::move(body));
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
	auto decl_list = std::make_unique<DeclListNode>();

	while (CurTok.type != EOF_TOK)
	{
		decl_list->addSub(parse_decl());
	}
	assert(decl_list->children.size() > 0);
	// TODO error if no main
	return decl_list;
}


// program ::= extern_list decl_list | decl_list
static std::unique_ptr<ProgramNode> parse_program()
{
	return std::make_unique<ProgramNode>(parse_extern_list(), parse_decl_list());
}


static std::unique_ptr<ProgramNode> parser()
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
	std::string pre = "";

	for (int i = 0; i < level; ++i)
	{
		pre += '\t';
	}
	std::cout << pre << root->to_string() << '\n';
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

static Value *int_cmp_zero(Value *val)
{
	auto Z = ConstantInt::get(Type::getInt32Ty(TheContext), 0, true);

	return Builder.CreateICmpEQ(val, Z, "intzcmptmp");
}


static Value *float_cmp_zero(Value *val)
{
	auto Z = ConstantFP::get(Type::getFloatTy(TheContext), 0.0f);

	return Builder.CreateFCmpUEQ(val, Z, "floatzcmptmp");
}


static Value *convert_float_to_int(Value *val)
{
	return Builder.CreateFPToSI(val, Type::getInt32Ty(TheContext), "float2inttmp");
}


static Value *convert_bool_to_int(Value *val)
{
	return Builder.CreateZExt(val, Type::getInt32Ty(TheContext), "bool2inttmp");
}


static Value *convert_int_to_bool(Value *val)
{
	auto current_parent = Builder.GetInsertBlock()->getParent();
	auto cmp            = Builder.CreateICmpEQ(val, Builder.getFalse(), "zcmptmp");
	auto then_block     = BasicBlock::Create(TheContext, "then", current_parent);
	auto else_block     = BasicBlock::Create(TheContext, "else", current_parent);
	auto merge_block    = BasicBlock::Create(TheContext, "ifcont", current_parent);

	Builder.CreateCondBr(cmp, then_block, else_block);

	Builder.SetInsertPoint(then_block);
	auto then_v = ConstantInt::get(Type::getInt1Ty(TheContext), 0, true);

	Builder.CreateBr(merge_block);

	Builder.SetInsertPoint(else_block);
	auto else_v = ConstantInt::get(Type::getInt1Ty(TheContext), 1, true);

	Builder.CreateBr(merge_block);

	Builder.SetInsertPoint(merge_block);
	auto phi = Builder.CreatePHI(Type::getInt1Ty(TheContext), 2, "int2booltmp");

	phi->addIncoming(then_v, then_block);
	phi->addIncoming(else_v, else_block);

	return phi;
}


static Value *convert_float_to_bool(Value *val)
{
	auto current_parent = Builder.GetInsertBlock()->getParent();
	auto cmp            = float_cmp_zero(val);
	auto then_block     = BasicBlock::Create(TheContext, "then", current_parent);
	auto else_block     = BasicBlock::Create(TheContext, "else", current_parent);
	auto merge_block    = BasicBlock::Create(TheContext, "ifcont", current_parent);

	Builder.CreateCondBr(cmp, then_block, else_block);


	Builder.SetInsertPoint(then_block);
	auto then_v = ConstantInt::get(Type::getInt1Ty(TheContext), 0.0f);

	Builder.CreateBr(merge_block);

	Builder.SetInsertPoint(else_block);
	auto else_v = ConstantInt::get(Type::getInt1Ty(TheContext), 1.0f);

	Builder.CreateBr(merge_block);

	Builder.SetInsertPoint(merge_block);
	auto phi = Builder.CreatePHI(Type::getInt1Ty(TheContext), 2, "float2booltmp");

	phi->addIncoming(then_v, then_block);
	phi->addIncoming(else_v, else_block);

	return phi;
}


static Value *convert_int_to_float(Value *val)
{
	return Builder.CreateSIToFP(val, Type::getFloatTy(TheContext), "int2floattmp");
}


static Value *convert_bool_to_float(Value *val)
{
	auto current_parent = Builder.GetInsertBlock()->getParent();
	auto cmp            = val;
	auto then_block     = BasicBlock::Create(TheContext, "then", current_parent);
	auto else_block     = BasicBlock::Create(TheContext, "else", current_parent);
	auto merge_block    = BasicBlock::Create(TheContext, "ifcont", current_parent);

	Builder.CreateCondBr(cmp, then_block, else_block);

	Builder.SetInsertPoint(then_block);
	auto then_v = ConstantFP::get(Type::getFloatTy(TheContext), 1.0f);

	Builder.CreateBr(merge_block);

	Builder.SetInsertPoint(else_block);
	auto else_v = ConstantFP::get(Type::getFloatTy(TheContext), 0.0f);

	Builder.CreateBr(merge_block);

	Builder.SetInsertPoint(merge_block);
	auto phi = Builder.CreatePHI(Type::getFloatTy(TheContext), 2, "bool2floattmp");

	phi->addIncoming(then_v, then_block);
	phi->addIncoming(else_v, else_block);

	return phi;
}


static Value *convert_value_to_int(Value *val)
{
	auto val_type = val->getType();

	if (val_type->isIntegerTy(1))
	{
		return convert_bool_to_int(val);
	}
	if (val_type->isIntegerTy(32))
	{
		return val;
	}
	if (val_type->isFloatTy())
	{
		return convert_float_to_int(val);
	}
	throw compiler_error("unknown type to convert to int");
}


static Value *convert_value_to_float(Value *val)
{
	auto val_type = val->getType();

	if (val_type->isIntegerTy(1))
	{
		return convert_bool_to_float(val);
	}
	if (val_type->isIntegerTy(32))
	{
		return convert_int_to_float(val);
	}
	if (val_type->isFloatTy())
	{
		return val;
	}
	throw compiler_error("unknown type to convert to float");
}


static Value *convert_value_to_bool(Value *val)
{
	auto val_type = val->getType();

	if (val_type->isIntegerTy(1))
	{
		return val;
	}
	if (val_type->isIntegerTy(32))
	{
		return convert_int_to_bool(val);
	}
	if (val_type->isFloatTy())
	{
		return convert_float_to_bool(val);
	}
	throw compiler_error("unknown type to convert to bool");
}


FunctionSignature::FunctionSignature(std::string name, const char return_type, std::unique_ptr<ASTnode> params)
	: name(name), return_type(return_type)
{
	children.push_back(std::move(params));
}


Function *FunctionSignature::codegen()
{
	std::vector<Type *> param_types;
	auto                fun_params = params();

	for (int i = 0; i < fun_params->param_count(); ++i)
	{
		auto param = fun_params->get(i);
		param_types.push_back(param->llvm_type());
	}
	assert(fun_params->param_count() == param_types.size());
	Type *return_ty;

	//todo change to use llvm types instead of homebrew
	switch (return_type)
	{
	case INT_TYPE:
		return_ty = Type::getInt32Ty(TheContext);
		break;

	case FLOAT_TYPE:
		return_ty = Type::getFloatTy(TheContext);
		break;

	case VOID_TYPE:
		return_ty = Type::getVoidTy(TheContext);
		break;

	case BOOL_TYPE:
		return_ty = Type::getInt1Ty(TheContext);
		break;

	default:
		return nullptr;
	}
	FunctionType *FT = FunctionType::get(return_ty, param_types, false);
	Function     *F  = Function::Create(FT, Function::ExternalLinkage, name, TheModule.get());
	int          i   = 0;

	for (auto& param: F->args())
	{
		param.setName(fun_params->get(i++)->name);
	}
	return F;
}


Value *ParenExprNode::codegen()
{
	return expr()->codegen();
}


Value *NegationNode::codegen()
{
	auto negatee_type = negatee()->expr_type();

	if (negatee_type == INT_TYPE)
	{
		auto Z = ConstantInt::get(Type::getInt32Ty(TheContext), 0, true);
		return Builder.CreateSub(Z, negatee()->codegen());
	}
	else if (negatee_type == FLOAT_TYPE)
	{
		return Builder.CreateFNeg(negatee()->codegen());
	}
	else if (negatee_type == BOOL_TYPE)
	{
		auto C = Builder.CreateBitCast(negatee()->codegen(), Type::getInt32Ty(TheContext));
		auto Z = ConstantInt::get(Type::getInt32Ty(TheContext), 0, true);
		return Builder.CreateSub(Z, C);
	}
	else
	{
		return nullptr;
	}
}


Value *VarExprNode::codegen()
{
	auto A = scope->getAddr(decl->var_name);
	Type *T;

	switch (decl->var_type)
	{
	case INT_TYPE:
		T = Type::getInt32Ty(TheContext);
		break;

	case FLOAT_TYPE:
		T = Type::getFloatTy(TheContext);
		break;

	case BOOL_TYPE:
		T = Type::getInt1Ty(TheContext);
		break;

	default:
		throw compiler_error("ERROR 231");
	}
	return Builder.CreateLoad(T, A);
}


Value *WhileStmt::codegen()
{
}


Value *ProgramNode::codegen()
{
	Value *v;

	for (int i = 0; i < children.size(); ++i)
	{
		v = children[i]->codegen();
		if (!v)
		{
			return nullptr;
		}
	}
	return v;
}


Value *AssignmentLHS::codegen()
{
	return Builder.getTrue(); // nothing to do
}


Value *AssignmentExpr::codegen()
{
	auto  var_to_name = lhs()->var_to;
	auto  AssignmentValue       = rhs()->codegen();
	Value *CastedValue;

	// auto var_decl = scope->getDecl(var_to_name);

	if (!scope->hasName(var_to_name))
	{
		throw semantic_error(std::string() + "Variable '" + var_to_name + "' is being assigned but never declared.");
	}


	switch (scope->getDecl(var_to_name)->var_type)
	{
	case INT_TYPE:
		CastedValue = convert_value_to_int(AssignmentValue);
		break;

	case FLOAT_TYPE:
		CastedValue = convert_value_to_float(AssignmentValue);
		break;

	case BOOL_TYPE:
		CastedValue = convert_value_to_bool(AssignmentValue);
		break;
	}

	auto Address = scope->getAddr(var_to_name);

	return Builder.CreateStore(CastedValue, Address);
}


Value *ExternListNode::codegen()
{
	if (children.size())
	{
		Value *v;

		for (auto& c: children)
		{
			v = c->codegen();
			if (!v)
			{
				return nullptr;
			}
		}
		return v;
	}
	return Builder.getTrue();
}


static bool is_int(Value *v)
{
	return v->getType()->isIntegerTy(32);
}


static bool is_bool(Value *v)
{
	return v->getType()->isIntegerTy(1);
}


static bool is_float(Value *v)
{
	return v->getType()->isFloatTy();
}


Value *IfWithElseNode::codegen()
{
	auto  outer_function = Builder.GetInsertBlock()->getParent();
	auto  cond_v         = cond->codegen();
	Value *comparison;

	if (is_int(cond_v))
	{
		auto Z = ConstantInt::get(Type::getInt32Ty(TheContext), 0, true);
		comparison = Builder.CreateICmpNE(cond->codegen(), Z, "ifcond");
	}
	else if (is_float(cond_v))
	{
		auto Z = ConstantInt::get(Type::getInt1Ty(TheContext), 0, false);
		comparison = Builder.CreateFCmpUNE(cond->codegen(), Z, "ifcond");
	}
	else if (is_bool(cond_v))
	{
		auto Z = ConstantFP::get(Type::getFloatTy(TheContext), 0.0);
		comparison = Builder.CreateICmpNE(cond->codegen(), Z, "ifcond");
	}
	else
	{
		return nullptr;
	}
	auto then_block  = BasicBlock::Create(TheContext, "then", outer_function);
	auto else_block  = BasicBlock::Create(TheContext, "else");
	auto merge_block = BasicBlock::Create(TheContext, "ifcont");

	Builder.CreateCondBr(comparison, then_block, else_block);

	// Emit then body
	Builder.SetInsertPoint(then_block);
	then_body->codegen();
	Builder.CreateBr(merge_block);

	// Emit else body
	outer_function->getBasicBlockList().push_back(else_block);
	Builder.SetInsertPoint(else_block);
	else_body->codegen();
	Builder.CreateBr(merge_block);

	outer_function->getBasicBlockList().push_back(merge_block);
	Builder.SetInsertPoint(merge_block);
	return merge_block;
}


Value *ReturnValueNode::codegen()
{
	// todo jump up to function definition to find return type
	auto current_function = Builder.GetInsertBlock()->getParent();
	auto return_type      = current_function->getFunctionType()->getReturnType();

	if (return_type->isVoidTy())
	{
		throw semantic_error("Function declared as void cannot return a value");
	}

	auto  return_val = expr->codegen();
	Value *R;

	if (return_type->isIntegerTy(32))
	{
		R = convert_value_to_int(return_val);
	}
	else if (return_type->isIntegerTy(1))
	{
		R = convert_value_to_bool(return_val);
	}
	else if (return_type->isFloatTy())
	{
		R = convert_value_to_float(return_val);
	}
	else
	{
		throw compiler_error("COMPILER ERROR: 651");
	}

	return Builder.CreateRet(R);
}


Value *ReturnNothingNode::codegen()
{
	return Builder.CreateRetVoid();
}


Value *NullStmt::codegen()
{
	return ConstantInt::getTrue(TheContext);
}


BlockNode::BlockNode()
{
	_scope = VariableScope::inherit_vars(ActiveScopes.top());
	scope  = _scope.get();
}


Value *BlockNode::codegen()
{
	local_decls()->codegen();
	stmt_list()->codegen();
	return Builder.getTrue();
}


Value *IfStmt::codegen()
{
	auto  outer_function = Builder.GetInsertBlock()->getParent();
	auto  cond_type      = cond->expr_type();
	Value *comparison;

	if (cond_type == INT_TYPE)
	{
		auto Z = ConstantInt::get(Type::getInt32Ty(TheContext), 0, true);
		comparison = Builder.CreateICmpNE(cond->codegen(), Z);
	}
	else if (cond_type == FLOAT_TYPE)
	{
		auto Z = ConstantFP::get(Type::getFloatTy(TheContext), 0.0);
		comparison = Builder.CreateFCmpUNE(cond->codegen(), Z);
	}
	else if (cond_type == BOOL_TYPE)
	{
		auto Z = ConstantInt::get(Type::getInt1Ty(TheContext), 0, false);
		comparison = Builder.CreateICmpNE(cond->codegen(), Z);
	}
	else
	{
		return nullptr;
	}
	auto then_block = BasicBlock::Create(TheContext, "then", outer_function);
	auto after_if   = BasicBlock::Create(TheContext, "ifcont", outer_function);

	Builder.CreateCondBr(comparison, then_block, after_if);

	Builder.SetInsertPoint(then_block);
	then_body->codegen();
	Builder.CreateBr(after_if);

	outer_function->getBasicBlockList().push_back(after_if);
	Builder.SetInsertPoint(after_if);
	return Builder.getTrue();
}


Value *NotNode::codegen()
{
	auto negatee_type = negatee()->expr_type();

	if (negatee_type == BOOL_TYPE)
	{
		auto ONE = ConstantInt::get(Type::getInt1Ty(TheContext), 1, false);
		return Builder.CreateXor(negatee()->codegen(), ONE, "nottemp");
	}
	else if (negatee_type == INT_TYPE)
	{
		auto current_parent = Builder.GetInsertBlock()->getParent();
		auto cmp            = int_cmp_zero(negatee()->codegen());

		auto then_block  = BasicBlock::Create(TheContext, "then", current_parent); // boolean false
		auto else_block  = BasicBlock::Create(TheContext, "else", current_parent); // boolean true
		auto merge_block = BasicBlock::Create(TheContext, "merge", current_parent);
		Builder.CreateCondBr(cmp, else_block, then_block);

		Builder.SetInsertPoint(then_block);
		auto then_v = ConstantInt::get(Type::getInt1Ty(TheContext), 1, false);
		Builder.CreateBr(merge_block);

		Builder.SetInsertPoint(else_block);
		auto else_v = ConstantInt::get(Type::getInt1Ty(TheContext), 1, false);
		Builder.CreateBr(merge_block);

		auto phi = Builder.CreatePHI(Type::getInt1Ty(TheContext), 2, "nottmp");
		phi->addIncoming(then_v, then_block);
		phi->addIncoming(else_v, else_block);
		return phi;
	}
	else if (negatee_type == FLOAT_TYPE)
	{
		auto Z          = ConstantFP::get(Type::getFloatTy(TheContext), 0.0);
		auto comparison = Builder.CreateFCmpUEQ(negatee()->codegen(), Z, "zcmp");
		auto B1         = BasicBlock::Create(TheContext, "zcmptrue");
		auto B2         = BasicBlock::Create(TheContext, "zcmpfalse");
		return Builder.CreateCondBr(comparison, B1, B2);
	}
	else
	{
		return nullptr;
	}
}


/// CreateEntryBlockAlloca - Create an alloca instruction in the entry block of
/// the function.  This is used for mutable variables etc.
// Copied from Kaleidoscope tutorial
// https://releases.llvm.org/10.0.0/docs/tutorial/MyFirstLanguageFrontend/LangImpl07.html
static AllocaInst *CreateEntryBlockAlloca(Type *VarType, const std::string& VarName)
{
	auto        TheFunction = Builder.GetInsertBlock()->getParent();
	IRBuilder<> TmpB(&TheFunction->getEntryBlock(), TheFunction->getEntryBlock().begin());

	return TmpB.CreateAlloca(VarType, 0, VarName.c_str());
}


// Short-circuit evaluation of conjunction (boolean AND)
Value *ConjunctionNode::codegen()
{
	if (children.size() == 1)
	{
		return children[0].get()->codegen();
	}
	auto  current_parent = Builder.GetInsertBlock()->getParent();
	Value *Result        = CreateEntryBlockAlloca(Type::getInt1Ty(TheContext), "conj_result");
	auto  end_block      = BasicBlock::Create(TheContext, "conj_ret");
	auto  succ_block     = BasicBlock::Create(TheContext, "conj_succ");
	auto  next_block     = BasicBlock::Create(TheContext, "conj_cl2");

	// Short-circuit evaluaton. The result is presumed false and
	// only changed to true after all nodes have been visited. If
	// any of the nodes (i.e. clauses of the conjunction) are false
	// then the result is immediately returned on the discovery of
	// such node. At the end of none such clauses are found then
	// Result is changed to true and then returned.
	Builder.CreateStore(Builder.getFalse(), Result);

	// First iteration on first clause of disjunction (i.e. first clause of disjunction)
	auto current_block = BasicBlock::Create(TheContext, "conj_cl1");

	Builder.CreateCondBr(convert_value_to_bool(children[0]->codegen()), end_block, next_block);

	// Iterate over all inner elements
	for (int i = 0; i < children.size() - 1; ++i)
	{
		current_parent->getBasicBlockList().push_back(current_block);                                   // add previous block
		current_block = next_block;
		next_block    = BasicBlock::Create(TheContext, std::string("conj_cl") + std::to_string(i + 1)); // last claus in disjunction
		Builder.CreateCondBr(convert_value_to_bool(children[i]->codegen()), next_block, end_block);
	}

	// Last iteration, only happens for 2 or more elements
	if (children.size() > 1)
	{
		Builder.SetInsertPoint(next_block);
		Builder.CreateCondBr(convert_value_to_bool(children[children.size() - 1]->codegen()), end_block, succ_block);
		current_parent->getBasicBlockList().push_back(next_block);
	}
	// TODO check actually generates the right clause labels numbers 2..n (n=number of clauses in disjunction)

	// fail block sets return value to 0
	Builder.SetInsertPoint(succ_block);
	Builder.CreateStore(Builder.getFalse(), Result);
	Builder.CreateBr(end_block);
	current_parent->getBasicBlockList().push_back(succ_block);

	// finally read the value and return it
	Builder.SetInsertPoint(end_block);
	auto ret_tmp = Builder.CreateLoad(Type::getInt1Ty(TheContext), Result, "conj_val");

	current_parent->getBasicBlockList().push_back(end_block);
	return ret_tmp;
}


// Short-circuit evaluation of disjunction (boolean OR)
Value *DisjunctionNode::codegen()
{
	if (children.size() == 1)
	{
		return children[0].get()->codegen();
	}
	auto  current_parent = Builder.GetInsertBlock()->getParent();
	Value *Result        = CreateEntryBlockAlloca(Type::getInt1Ty(TheContext), "disj_result");
	auto  end_block      = BasicBlock::Create(TheContext, "disj_ret");
	auto  fail_block     = BasicBlock::Create(TheContext, "disj_fail");
	auto  next_block     = BasicBlock::Create(TheContext, "disj_cl2");

	Builder.CreateStore(Builder.getTrue(), Result);

	// First iteration on first clause of disjunction (i.e. first clause of disjunction)
	auto current_block = BasicBlock::Create(TheContext, "disj_cl1");

	Builder.CreateCondBr(convert_value_to_bool(children[0]->codegen()), end_block, next_block);

	// Iterate over all inner elements
	for (int i = 0; i < children.size() - 1; ++i)
	{
		current_parent->getBasicBlockList().push_back(current_block);                                   // add previous block
		current_block = next_block;
		next_block    = BasicBlock::Create(TheContext, std::string("disj_cl") + std::to_string(i + 1)); // last claus in disjunction
		Builder.CreateCondBr(convert_value_to_bool(children[i]->codegen()), end_block, next_block);
	}

	// Last iteration, only happens for 2 or more elements
	if (children.size() > 1)
	{
		Builder.SetInsertPoint(next_block);
		Builder.CreateCondBr(convert_value_to_bool(children[children.size() - 1]->codegen()), end_block, fail_block);
		current_parent->getBasicBlockList().push_back(next_block);
	}
	// TODO check actually generates the right clause labels numbers 2..n (n=number of clauses in disjunction)

	// fail block sets return value to 0
	Builder.SetInsertPoint(fail_block);
	Builder.CreateStore(Builder.getFalse(), Result);
	Builder.CreateBr(end_block);
	current_parent->getBasicBlockList().push_back(fail_block);

	// finally read the value and return it
	Builder.SetInsertPoint(end_block);
	auto ret_tmp = Builder.CreateLoad(Type::getInt1Ty(TheContext), Result, "disjval");

	current_parent->getBasicBlockList().push_back(end_block);
	return ret_tmp;
}


Function *FunDeclNode::codegen()
{
	Function *the_function = TheModule->getFunction(sig()->name);

	if (!the_function)
	{
		the_function = sig()->codegen();
	}

	if (!the_function)
	{
		return nullptr;
	}

	if (!the_function->empty())
	{
		throw semantic_error("Function " + sig()->name + " cannot be  redefined");
	}

	auto BB = BasicBlock::Create(TheContext, "entry", the_function);

	Builder.SetInsertPoint(BB);


	// clear named values map
	// for (arg in args) set name in values map

	body()->codegen();

	verifyFunction(*the_function);

	return the_function;
}


Value *FunCallNode::codegen()
{
	auto callee_function = TheModule->getFunction(fun_name);


	// todo when parsing check param names are unique

	return Builder.CreateCall(callee_function->getFunctionType(), callee_function, {}, "funcalltmp");
	// TODO ASAP finish this
}


Value *IntNode::codegen()
{
	return ConstantInt::get(Type::getInt32Ty(TheContext), get_int(), true);
}


Value *BoolNode::codegen()
{
	return ConstantInt::get(Type::getInt1Ty(TheContext), get_bool()?1:0, false);
}


Value *FloatNode::codegen()
{
	return ConstantFP::get(Type::getFloatTy(TheContext), get_float());
}


Value *OpMULT::codegen()
{
	auto L = lhs()->codegen(), R = rhs()->codegen();
	auto l_type = lhs()->expr_type(), r_type = rhs()->expr_type();

	if ((l_type == FLOAT_TYPE) || (r_type == FLOAT_TYPE))
	{
		L = convert_value_to_float(L);
		R = convert_value_to_float(R);
		return Builder.CreateFMul(L, R);
	}
	else if ((l_type == INT_TYPE) || (r_type == INT_TYPE))
	{
		L = convert_value_to_int(L);
		R = convert_value_to_int(R);
		return Builder.CreateMul(L, R);
	}
	return Builder.CreateMul(L, R);
}


AssignmentExpr::AssignmentExpr(VariableScope *scope, std::unique_ptr<ASTnode> lhs, std::unique_ptr<ASTnode> rhs)
	: scope(scope)
{
	children.push_back(std::move(lhs));
	children.push_back(std::move(rhs));
	if (this->rhs()->expr_type() == VOID_TYPE)
	{
		throw semantic_error(" void value not ignored as it ought to be"); // Same error as gcc
	}
}


Value *OpADD::codegen()
{
	auto L = lhs()->codegen(), R = rhs()->codegen();
	auto l_type = lhs()->expr_type(), r_type = rhs()->expr_type();

	if ((l_type == FLOAT_TYPE) || (r_type == FLOAT_TYPE))
	{
		L = convert_value_to_float(L);
		R = convert_value_to_float(R);
		return Builder.CreateFAdd(L, R);
	}
	else if ((l_type == INT_TYPE) || (r_type == INT_TYPE))
	{
		L = convert_value_to_int(L);
		R = convert_value_to_int(R);
		return Builder.CreateAdd(L, R);
	}
	return Builder.CreateAdd(L, R);
}


Value *OpDIV::codegen()
{
	auto L = lhs()->codegen(), R = rhs()->codegen();
	auto l_type = lhs()->expr_type(), r_type = rhs()->expr_type();

	if ((l_type == FLOAT_TYPE) || (r_type == FLOAT_TYPE))
	{
		L = convert_value_to_float(L);
		R = convert_value_to_float(R);
		return Builder.CreateFDiv(L, R);
	}
	else if ((l_type == INT_TYPE) || (r_type == INT_TYPE))
	{
		L = convert_value_to_int(L);
		R = convert_value_to_int(R);
		return Builder.CreateSDiv(L, R);
	}
	return Builder.CreateSDiv(L, R);
}


Value *OpMODULO::codegen()
{
	auto L = lhs()->codegen(), R = rhs()->codegen();

	return Builder.CreateSRem(L, R);
}


Value *OpSUB::codegen()
{
	auto L = lhs()->codegen(), R = rhs()->codegen();
	auto l_type = lhs()->expr_type(), r_type = rhs()->expr_type();

	if ((l_type == FLOAT_TYPE) || (r_type == FLOAT_TYPE))
	{
		L = convert_value_to_float(L);
		R = convert_value_to_float(R);
		return Builder.CreateFSub(L, R);
	}
	else if ((l_type == INT_TYPE) || (r_type == INT_TYPE))
	{
		L = convert_value_to_int(L);
		R = convert_value_to_int(R);
		return Builder.CreateSub(L, R);
	}
	return Builder.CreateSub(L, R);
}


Value *OpEQ::codegen()
{
	auto L = lhs()->codegen(), R = rhs()->codegen();
	auto l_type = lhs()->expr_type(), r_type = rhs()->expr_type();

	if ((l_type == FLOAT_TYPE) || (r_type == FLOAT_TYPE))
	{
		L = convert_value_to_float(L);
		R = convert_value_to_float(R);
		return Builder.CreateFCmpUEQ(L, R);
	}
	else if ((l_type == INT_TYPE) || (r_type == INT_TYPE))
	{
		if (l_type == BOOL_TYPE)
		{
			L = convert_value_to_int(L);
		}
		if (r_type == BOOL_TYPE)
		{
			R = convert_value_to_int(R);
		}
		return Builder.CreateICmpEQ(L, R);
	}
	return Builder.CreateICmpEQ(L, R);
}


Value *OpNE::codegen()
{
	auto L = lhs()->codegen(), R = rhs()->codegen();
	auto l_type = lhs()->expr_type(), r_type = rhs()->expr_type();

	if ((l_type == FLOAT_TYPE) || (r_type == FLOAT_TYPE))
	{
		L = convert_value_to_float(L);
		R = convert_value_to_float(R);
		return Builder.CreateFCmpUNE(L, R);
	}
	else if ((l_type == INT_TYPE) || (r_type == INT_TYPE))
	{
		if (l_type == BOOL_TYPE)
		{
			L = convert_value_to_int(L);
		}
		if (r_type == BOOL_TYPE)
		{
			R = convert_value_to_int(R);
		}
		return Builder.CreateICmpNE(L, R);
	}
	return Builder.CreateICmpNE(L, R);
}


Value *OpLE::codegen()
{
	auto L = lhs()->codegen(), R = rhs()->codegen();
	auto l_type = lhs()->expr_type(), r_type = rhs()->expr_type();

	if ((l_type == FLOAT_TYPE) || (r_type == FLOAT_TYPE))
	{
		L = convert_value_to_float(L);
		R = convert_value_to_float(R);
		return Builder.CreateFCmpULE(L, R);
	}
	else if ((l_type == INT_TYPE) || (r_type == INT_TYPE))
	{
		L = convert_value_to_int(L);
		R = convert_value_to_int(R);
		return Builder.CreateICmpSLE(L, R);
	}
	return Builder.CreateICmpULE(L, R);
}


Value *VariableScope::getAddr(std::string var_name)
{
	if (hasLocalName(var_name))
	{
		auto i = local_addrs.find(var_name);
		return i != local_addrs.end()? i->second: nullptr;
	}
	if (inherited)
	{
		return inherited->getAddr(var_name);
	}
	return nullptr;
}


Value *OpGE::codegen()
{
	auto L = lhs()->codegen(), R = rhs()->codegen();
	auto l_type = lhs()->expr_type(), r_type = rhs()->expr_type();

	if ((l_type == FLOAT_TYPE) || (r_type == FLOAT_TYPE))
	{
		L = convert_value_to_float(L);
		R = convert_value_to_float(R);
		return Builder.CreateFCmpUGE(L, R);
	}
	else if ((l_type == INT_TYPE) || (r_type == INT_TYPE))
	{
		L = convert_value_to_int(L);
		R = convert_value_to_int(R);
		return Builder.CreateICmpSGE(L, R);
	}
	return Builder.CreateICmpUGE(L, R);
}


Value *OpGT::codegen()
{
	auto L = lhs()->codegen(), R = rhs()->codegen();
	auto l_type = lhs()->expr_type(), r_type = rhs()->expr_type();

	if ((l_type == FLOAT_TYPE) || (r_type == FLOAT_TYPE))
	{
		L = convert_value_to_float(L);
		R = convert_value_to_float(R);
		return Builder.CreateFCmpUGT(L, R);
	}
	else if ((l_type == INT_TYPE) || (r_type == INT_TYPE))
	{
		L = convert_value_to_int(L);
		R = convert_value_to_int(R);
		return Builder.CreateICmpSGT(L, R);
	}
	return Builder.CreateICmpUGT(L, R);
}


void VariableScope::setAddr(std::string var_name, Value *addr)
{
	if (hasLocalName(var_name))
	{
		local_addrs[var_name] = addr;
	}
	else if (inherited)
	{
		inherited->setAddr(var_name, addr);
	}
	else
	{
		throw compiler_error("COMPILER ERROR: 452");
	}
}


Value *ParamNode::codegen()
{
	return nullptr;
}


Value *DeclListNode::codegen()
{
	Value *v;

	for (int i = 0; i < children.size(); ++i)
	{
		v = children[i]->codegen();
		if (!v)
		{
			return nullptr;
		}
	}
	return v;
}


Value *StmtListNode::codegen()
{
	if (children.size())
	{
		Value *v;

		for (int i = 0; i < children.size(); ++i)
		{
			v = children[i]->codegen();
			if (!v)
			{
				return nullptr;
			}
		}
		return v;
	}
	return Builder.getTrue();
}


Value *VarDeclNode::codegen()
{
	if (is_global)
	{
		if (TheModule->getGlobalVariable(var_name))
		{
			throw semantic_error(std::string() + "Tried to declare global variable " + var_name + " twice");
		}
		Constant *V;
		Type     *T;
		switch (var_type)
		{
		case INT_TYPE:
			T = Type::getInt32Ty(TheContext);
			V = ConstantInt::get(Type::getInt32Ty(TheContext), 0, true);
			break;

		case BOOL_TYPE:
			T = Type::getInt1Ty(TheContext);
			V = ConstantInt::get(Type::getInt1Ty(TheContext), 0, false);
			break;

		case FLOAT_TYPE:
			T = Type::getFloatTy(TheContext);
			V = ConstantFP::get(Type::getFloatTy(TheContext), 0.0f);
			break;

		default:
			throw compiler_error(std::string() + "Unknown variable type " + var_type);
		}
		auto Address = new GlobalVariable(*TheModule, T, false, GlobalValue::ExternalLinkage, V, var_name);
		scope->setAddr(var_name, Address);
		return Address;
	}
	else
	{
		Type *T;
		switch (var_type)
		{
		case INT_TYPE:
			T = Type::getInt32Ty(TheContext);
			break;

		case BOOL_TYPE:
			T = Type::getInt1Ty(TheContext);
			break;

		case FLOAT_TYPE:
			T = Type::getFloatTy(TheContext);
			break;

		default:
			throw compiler_error("Invalid type for variable " + var_name);
		}
		auto Address = CreateEntryBlockAlloca(T, var_name);
		scope->setAddr(var_name, Address);
		return Address;
	}
}


// todo handle empty function body when codegening

Value *LocalDeclsNode::codegen()
{
	if (children.size())
	{
		Value *v;

		for (int i = 0; i < children.size(); ++i)
		{
			v = children[i]->codegen();
			if (!v)
			{
				return nullptr;
			}
		}
		return v;
	}
	return Builder.getTrue();
}


Value *ParamsNode::codegen()
{
	return nullptr;
}


Value *OpLT::codegen()
{
	auto L = lhs()->codegen(), R = rhs()->codegen();
	auto l_type = lhs()->expr_type(), r_type = rhs()->expr_type();

	if ((l_type == FLOAT_TYPE) || (r_type == FLOAT_TYPE))
	{
		if (l_type == INT_TYPE)
		{
			L = convert_int_to_float(L);
		}
		else if (l_type == BOOL_TYPE)
		{
			L = convert_bool_to_float(L);
		}
		if (r_type == INT_TYPE)
		{
			R = convert_int_to_float(R);
		}
		else if (l_type == BOOL_TYPE)
		{
			R = convert_bool_to_float(R);
		}
		return Builder.CreateFCmpULT(L, R);
	}
	else if ((l_type == INT_TYPE) || (r_type == INT_TYPE))
	{
		if (l_type == BOOL_TYPE)
		{
			L = Builder.CreateBitCast(L, Type::getInt32Ty(TheContext));
		}
		if (r_type == BOOL_TYPE)
		{
			R = Builder.CreateBitCast(R, Type::getInt32Ty(TheContext));
		}
		return Builder.CreateICmpSLT(L, R);
	}
	return Builder.CreateICmpULT(L, R);
}


Type *ParamNode::llvm_type()
{
	switch (type)
	{
	case FLOAT_TYPE:
		return Type::getFloatTy(TheContext);

	case BOOL_TYPE:
		return Type::getInt1Ty(TheContext);

	case INT_TYPE:
		return Type::getInt32Ty(TheContext);

	default:
		return nullptr;
	}
}


void VariableScope::setDecl(std::string var_name, VarDeclNode *decl)
{
	if (hasLocalName(var_name))
	{
		throw semantic_error(std::string() + "attempted to define " + var_name + " but has been already defined in this scope");
	}
	local_decls[var_name] = decl;
}


char FunCallNode::expr_type()
{
	if (DefinedFunctions.count(fun_name))
	{
		return DefinedFunctions[fun_name]->sig()->return_type;
	}
	throw semantic_error("Calling function " + fun_name + " which hasn't been defined");
}


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
		std::cout << "Usage: ./code InputFile\n";
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
	std::cerr << "Lexer Finished\n";

	// Make the module, which holds all the code.
	TheModule = std::make_unique<Module>("mini-c", TheContext);

	// Run the parser now.
	try
	{
		auto program = parser();
		std::cerr << "Parsing Finished\n";
		print_ast(program.get());
		program->codegen();
	}
	catch (syntax_error& e)
	{
		std::cerr << "Parsing ERROR on line " << e.lineNo << " column " << columnNo << " received token " << e.erroneous_token << std::endl;
		std::cerr << e.what() << '\n';
	}
	catch (semantic_error& e)
	{
		std::cerr << "Semantic ERROR on line " << e.lineNo << " column " << columnNo << " received token " << e.erroneous_token << std::endl;
		std::cerr << e.what() << '\n';
	}
	catch (compiler_error& e)
	{
		std::cerr << "Compiler ERROR on line " << e.lineNo << " column " << columnNo << " received token " << e.erroneous_token << std::endl;
		std::cerr << e.what() << '\n';
	}

	//********************* Start printing final IR **************************
	// Print out all of the generated code into a file called output.ll
	auto            Filename = "output.ll";
	std::error_code EC;
	raw_fd_ostream  dest(Filename, EC, sys::fs::F_None);

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
