#include "conversions.hpp"

#include "ExprNode.hpp"
#include "ValidTypes.hpp"
#include "the_externs.hpp"

Value *int_cmp_zero(Value *val, Twine const &label = "intzcmptmp")
{
    return Builder.CreateICmpEQ(val, IntValues::Zero(), label);
}

Value *float_cmp_zero(Value *val, Twine const &label = "floatzcmptmp")
{
    return Builder.CreateFCmpUEQ(val, FloatValues::Zero(), label);
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
    return Builder.CreateICmpNE(expr, IntValues::Zero(), name);
}

template <>
[[nodiscard]] Value *create_true_cmp<bool>(Value *expr, Twine const &name)
{
    return Builder.CreateICmpNE(expr, BoolValues::False(), name);
}

template <>
[[nodiscard]] Value *create_true_cmp<float>(Value *expr, Twine const &name)
{
    return Builder.CreateFCmpUNE(expr, FloatValues::Zero(), name);
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
