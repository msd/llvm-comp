#include "llvm/IR/IRBuilder.h"
#include "llvm/IR/Type.h"

#include "the_externs.hpp"
#include "my_errors.hpp"

semantic_error::semantic_error(string err_msg) : err_msg(move(err_msg)), lineNo(CurTok.lineNo), columnNo(CurTok.columnNo), erroneous_token(CurTok.lexeme) {}

syntax_error::syntax_error(string err_msg) : err_msg(move(err_msg)), lineNo(CurTok.lineNo), columnNo(CurTok.columnNo), erroneous_token(CurTok.lexeme) {}

compiler_error::compiler_error() : err_msg("Unknown compiler error"), lineNo(CurTok.lineNo), columnNo(CurTok.columnNo), erroneous_token(CurTok.lexeme) {}
compiler_error::compiler_error(string err_msg) : err_msg(move(err_msg)), lineNo(CurTok.lineNo), columnNo(CurTok.columnNo), erroneous_token(CurTok.lexeme) {}

AllocaInst *CreateEntryBlockAlloca(Type *VarType, const string& VarName)
{
	auto        TheFunction = Builder.GetInsertBlock()->getParent();
	IRBuilder<> TmpB(&TheFunction->getEntryBlock(), TheFunction->getEntryBlock().begin());

	return TmpB.CreateAlloca(VarType, 0, VarName.c_str());
}

string type_to_str(const char type)
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