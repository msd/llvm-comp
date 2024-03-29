#include "my_errors.hpp"

#include "llvm/IR/IRBuilder.h"

#include "the_externs.hpp"

semantic_error::semantic_error(std::string err_msg, TOKEN *token)
    : err_msg(std::move(err_msg)), lineNo(token->lineNo),
      columnNo(token->columnNo), erroneous_token(token->lexeme)
{
}

syntax_error::syntax_error(std::string err_msg, TOKEN *token)
    : err_msg(std::move(err_msg)), lineNo(token->lineNo),
      columnNo(token->columnNo), erroneous_token(token->lexeme)
{
}

compiler_error::compiler_error()
    : err_msg("Unknown compiler error"), lineNo(1), columnNo(1)
{
}
compiler_error::compiler_error(std::string err_msg, TOKEN *token)
    : err_msg(std::move(err_msg)), lineNo(token->lineNo),
      columnNo(token->columnNo), erroneous_token(token->lexeme)
{
}

AllocaInst *CreateEntryBlockAlloca(Type *VarType, const std::string &VarName)
{
    auto TheFunction = Builder.GetInsertBlock()->getParent();
    IRBuilder<> TmpB(&TheFunction->getEntryBlock(),
                     TheFunction->getEntryBlock().begin());

    return TmpB.CreateAlloca(VarType, 0, VarName);
}

std::string type_to_str(const char type)
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
