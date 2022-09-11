#include "FunctionSignature.hpp"

#include "ValidTypes.hpp"
#include "the_externs.hpp"

FunctionSignature::FunctionSignature(Parser *parser, string name, const char return_type,
                                     unique_ptr<ASTnode> params)
    : ASTnode(parser), name(name), return_type(return_type)
{
    children.push_back(move(params));
}

Function *FunctionSignature::codegen()
{
    vector<Type *> param_types;
    auto fun_params = params();

    for (int i = 0; i < fun_params->param_count(); ++i)
    {
        auto param = fun_params->get(i);
        param_types.push_back(param->llvm_type());
    }
    assert(fun_params->param_count() == param_types.size());
    Type *return_ty;

    // todo change to use llvm types instead of homebrew
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
    Function *F =
        Function::Create(FT, Function::ExternalLinkage, name, TheModule.get());
    int i = 0;

    for (auto &param : F->args())
    {
        param.setName(fun_params->get(i++)->name);
    }
    return F;
}
