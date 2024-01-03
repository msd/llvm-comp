#include "is_type.hpp"

#include "the_externs.hpp"

bool is_int(Value *v)
{
    return v->getType()->isIntegerTy(INT_BITS_COUNT);
}

bool is_bool(Value *v)
{
    return v->getType()->isIntegerTy(BOOL_BITS_COUNT);
}

bool is_float(Value *v)
{
    return v->getType()->isFloatTy();
}

template <> Value *create_true_cmp<int>(Value *expr, Twine const &name)
{
    auto Z = ConstantInt::get(Type::getInt32Ty(TheContext), 0, true);
    return Builder.CreateICmpNE(expr, Z, name);
}

template <> Value *create_true_cmp<bool>(Value *expr, Twine const &name)
{
    auto Z = ConstantInt::get(Type::getInt1Ty(TheContext), 0, false);
    return Builder.CreateICmpNE(expr, Z, name);
}

template <> Value *create_true_cmp<float>(Value *expr, Twine const &name)
{
    auto Z = ConstantFP::get(Type::getFloatTy(TheContext), 0.0);
    return Builder.CreateFCmpUNE(expr, Z, name);
}

Value *create_expr_true_check(ExprNode *expr)
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
