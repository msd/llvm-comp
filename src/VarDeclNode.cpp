#include "VarDeclNode.hpp"

#include "llvm/IR/IRBuilder.h"

#include "ValidTypes.hpp"
#include "VariableScope.hpp"
#include "my_errors.hpp"
#include "the_externs.hpp"

Value *VarDeclNode::codegen()
{
    if (is_global)
    {
        if (TheModule->getGlobalVariable(var_name))
        {
            throw semantic_error(string() +
                                 "Tried to declare global variable " +
                                 var_name + " twice");
        }
        Constant *V;
        Type *T;
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
            throw compiler_error(string() + "Unknown variable type " +
                                 var_type);
        }
        auto Address = new GlobalVariable(
            *TheModule, T, false, GlobalValue::ExternalLinkage, V, var_name);
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
