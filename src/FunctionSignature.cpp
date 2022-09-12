#include "FunctionSignature.hpp"

#include "ValidTypes.hpp"
#include "conversions.hpp"
#include "the_externs.hpp"

FunctionSignature::FunctionSignature(Parser *parser, string name,
                                     const char return_type,
                                     unique_ptr<ASTnode> params)
    : ASTnode(parser), name(name), return_type(return_type)
{
    children.push_back(move(params));
}

vector<Type *> FunctionSignature::param_types()
{
    vector<Type *> param_types;
    auto fun_params = params();

    for (int i = 0; i < fun_params->param_count(); ++i)
    {
        auto param = fun_params->get(i);
        param_types.push_back(param->llvm_type());
    }
    assert(fun_params->param_count() == param_types.size());
    return param_types;
}

Function *FunctionSignature::codegen()
{
    auto return_type_llvm = type_homebrew_to_llvm(return_type);

    FunctionType *FT =
        FunctionType::get(return_type_llvm, param_types(), false);
    Function *F =
        Function::Create(FT, Function::ExternalLinkage, name, TheModule.get());

    int i = 0;
    const auto fun_params = params();
    for (auto &param : F->args())
    {
        param.setName(fun_params->get(i++)->name);
    }
    return F;
}
