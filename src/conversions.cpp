#include "conversions.hpp"

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
