#include "is_type.hpp"

#include "the_externs.hpp"

bool is_int(Value *v)
{
    return v->getType()->isIntegerTy(32);
}

bool is_bool(Value *v)
{
    return v->getType()->isIntegerTy(1);
}

bool is_float(Value *v)
{
    return v->getType()->isFloatTy();
}

Value *create_expr_true_check(ExprNode *expr)
{
    auto cond_type = expr->expr_type();

    switch (cond_type)
    {
    case INT_TYPE: {
        auto Z = ConstantInt::get(Type::getInt32Ty(TheContext), 0, true);
        return Builder.CreateICmpNE(expr->codegen(), Z);
    }
    case FLOAT_TYPE: {
        auto Z = ConstantFP::get(Type::getFloatTy(TheContext), 0.0);
        return Builder.CreateFCmpUNE(expr->codegen(), Z);
    }
    case BOOL_TYPE: {
        auto Z = ConstantInt::get(Type::getInt1Ty(TheContext), 0, false);
        return Builder.CreateICmpNE(expr->codegen(), Z);
    }
    default:
        throw compiler_error(
            "expr type unrecognized when creating true check (unknown type: " +
            std::to_string(cond_type) + ")");
    }
}
