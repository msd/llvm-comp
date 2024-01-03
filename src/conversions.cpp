#include "conversions.hpp"

#include "ExprNode.hpp"
#include "ValidTypes.hpp"
#include "the_externs.hpp"

Value *int_cmp_zero(Value *val)
{
    auto Z = ConstantInt::get(Type::getInt32Ty(TheContext), 0, true);

    return Builder.CreateICmpEQ(val, Z, "intzcmptmp");
}

Value *float_cmp_zero(Value *val)
{
    auto Z = ConstantFP::get(Type::getFloatTy(TheContext), 0.0f);

    return Builder.CreateFCmpUEQ(val, Z, "floatzcmptmp");
}

Type *type_homebrew_to_llvm(char type)
{
    // todo change to use llvm types instead of homebrew
    switch (type)
    {
    case INT_TYPE:
        return Type::getInt32Ty(TheContext);

    case FLOAT_TYPE:
        return Type::getFloatTy(TheContext);

    case VOID_TYPE:
        return Type::getVoidTy(TheContext);

    case BOOL_TYPE:
        return Type::getInt1Ty(TheContext);
    }
    throw std::runtime_error("invalid type received");
};

template <>
[[nodiscard]] Value *create_true_cmp<int>(Value *expr, Twine const &name)
{
    auto Z = ConstantInt::get(Type::getInt32Ty(TheContext), 0, true);
    return Builder.CreateICmpNE(expr, Z, name);
}

template <>
[[nodiscard]] Value *create_true_cmp<bool>(Value *expr, Twine const &name)
{
    auto Z = ConstantInt::get(Type::getInt1Ty(TheContext), 0, false);
    return Builder.CreateICmpNE(expr, Z, name);
}

template <>
[[nodiscard]] Value *create_true_cmp<float>(Value *expr, Twine const &name)
{
    auto Z = ConstantFP::get(Type::getFloatTy(TheContext), 0.0);
    return Builder.CreateFCmpUNE(expr, Z, name);
}

[[nodiscard]] Value *create_expr_true_check(ExprNode *expr)
{
    auto cond_type = expr->expr_type();

    switch (cond_type)
    {
    case INT_TYPE:
        return create_true_cmp<int>(expr->codegen());
    case FLOAT_TYPE:
        return create_true_cmp<float>(expr->codegen());
    case BOOL_TYPE:
        return create_true_cmp<bool>(expr->codegen());
    default:
        throw compiler_error(
            "expr type unrecognized when creating true check (unknown type: " +
            std::to_string(cond_type) + ")");
    }
}
