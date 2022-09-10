#include "VarExprNode.hpp"

#include "ValidTypes.hpp"
#include "VarDeclNode.hpp"
#include "VariableScope.hpp"
#include "my_errors.hpp"
#include "the_externs.hpp"

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

char VarExprNode::expr_type()
{
    return decl->var_type;
}
