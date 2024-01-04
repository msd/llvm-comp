#include "FunctionSignature.hpp"

#include "comparisons.hpp"
#include "the_externs.hpp"

FunctionSignature::FunctionSignature(Parser *parser, std::string name,
                                     const char return_type,
                                     std::unique_ptr<ASTnode> params)
    : ASTnode(parser), name(name), return_type(return_type)
{
    children.push_back(std::move(params));
}

std::vector<Type *> FunctionSignature::param_types()
{
    std::vector<Type *> list;
    auto fun_params = params();

    for (int i = 0; i < fun_params->param_count(); ++i)
    {
        auto param = fun_params->get(i);
        list.push_back(param->llvm_type());
    }
    assert(fun_params->param_count() == list.size());
    return list;
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
