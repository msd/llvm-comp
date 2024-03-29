#include "Ops.hpp"

#include "ValidTypes.hpp"
#include "coercion.hpp"
#include "the_externs.hpp"
#include <llvm/ADT/STLExtras.h>

Value *OpMULT::codegen()
{
    auto L = lhs()->codegen(), R = rhs()->codegen();
    auto l_type = lhs()->expr_type(), r_type = rhs()->expr_type();

    if ((l_type == FLOAT_TYPE) || (r_type == FLOAT_TYPE))
    {
        L = convert_value_to_float(L);
        R = convert_value_to_float(R);
        return Builder.CreateFMul(L, R);
    }

    if ((l_type == INT_TYPE) || (r_type == INT_TYPE))
    {
        L = convert_value_to_int(L);
        R = convert_value_to_int(R);
        return Builder.CreateMul(L, R);
    }

    // both bools
    return Builder.CreateMul(L, R);
}

Value *OpADD::codegen()
{
    auto L = lhs()->codegen(), R = rhs()->codegen();
    auto l_type = lhs()->expr_type(), r_type = rhs()->expr_type();

    if ((l_type == FLOAT_TYPE) || (r_type == FLOAT_TYPE))
    {
        L = convert_value_to_float(L);
        R = convert_value_to_float(R);
        return Builder.CreateFAdd(L, R);
    }

    if ((l_type == INT_TYPE) || (r_type == INT_TYPE))
    {
        L = convert_value_to_int(L);
        R = convert_value_to_int(R);
        return Builder.CreateAdd(L, R);
    }

    // both bools
    return Builder.CreateAdd(L, R);
}

Value *OpDIV::codegen()
{
    auto L = lhs()->codegen(), R = rhs()->codegen();
    auto l_type = lhs()->expr_type(), r_type = rhs()->expr_type();

    if ((l_type == FLOAT_TYPE) || (r_type == FLOAT_TYPE))
    {
        L = convert_value_to_float(L);
        R = convert_value_to_float(R);
        return Builder.CreateFDiv(L, R);
    }

    if ((l_type == INT_TYPE) || (r_type == INT_TYPE))
    {
        L = convert_value_to_int(L);
        R = convert_value_to_int(R);
        return Builder.CreateSDiv(L, R);
    }

    // both bools
    return Builder.CreateSDiv(L, R);
}

Value *OpMODULO::codegen()
{
    auto L = lhs()->codegen(), R = rhs()->codegen();

    return Builder.CreateSRem(L, R);
}

Value *OpSUB::codegen()
{
    auto L = lhs()->codegen(), R = rhs()->codegen();
    auto l_type = lhs()->expr_type(), r_type = rhs()->expr_type();

    if ((l_type == FLOAT_TYPE) || (r_type == FLOAT_TYPE))
    {
        L = convert_value_to_float(L);
        R = convert_value_to_float(R);
        return Builder.CreateFSub(L, R);
    }

    if ((l_type == INT_TYPE) || (r_type == INT_TYPE))
    {
        L = convert_value_to_int(L);
        R = convert_value_to_int(R);
        return Builder.CreateSub(L, R);
    }

    // both bools
    return Builder.CreateSub(L, R);
}

Value *OpEQ::codegen()
{
    auto L = lhs()->codegen(), R = rhs()->codegen();
    auto l_type = lhs()->expr_type(), r_type = rhs()->expr_type();

    if ((l_type == FLOAT_TYPE) || (r_type == FLOAT_TYPE))
    {
        L = convert_value_to_float(L);
        R = convert_value_to_float(R);
        return Builder.CreateFCmpUEQ(L, R);
    }

    if ((l_type == INT_TYPE) || (r_type == INT_TYPE))
    {
        if (l_type == BOOL_TYPE)
        {
            L = convert_value_to_int(L);
        }
        if (r_type == BOOL_TYPE)
        {
            R = convert_value_to_int(R);
        }
        return Builder.CreateICmpEQ(L, R);
    }

    // both bools
    return Builder.CreateICmpEQ(L, R);
}

Value *OpNE::codegen()
{
    // promote_types(lhs(), rhs());
    auto L = lhs()->codegen(), R = rhs()->codegen();
    auto l_type = lhs()->expr_type(), r_type = rhs()->expr_type();

    if ((l_type == FLOAT_TYPE) || (r_type == FLOAT_TYPE))
    {
        L = convert_value_to_float(L);
        R = convert_value_to_float(R);
        return Builder.CreateFCmpUNE(L, R);
    }
    if ((l_type == INT_TYPE) || (r_type == INT_TYPE))
    {
        if (l_type == BOOL_TYPE)
        {
            L = convert_value_to_int(L);
        }
        if (r_type == BOOL_TYPE)
        {
            R = convert_value_to_int(R);
        }
        return Builder.CreateICmpNE(L, R);
    }
    return Builder.CreateICmpNE(L, R);
}

Value *OpLE::codegen()
{
    auto L = lhs()->codegen(), R = rhs()->codegen();
    auto l_type = lhs()->expr_type(), r_type = rhs()->expr_type();

    if ((l_type == FLOAT_TYPE) || (r_type == FLOAT_TYPE))
    {
        L = convert_value_to_float(L);
        R = convert_value_to_float(R);
        return Builder.CreateFCmpULE(L, R);
    }

    if ((l_type == INT_TYPE) || (r_type == INT_TYPE))
    {
        L = convert_value_to_int(L);
        R = convert_value_to_int(R);
        return Builder.CreateICmpSLE(L, R);
    }

    // both bools
    return Builder.CreateICmpULE(L, R);
}

Value *OpGE::codegen()
{
    auto L = lhs()->codegen(), R = rhs()->codegen();
    auto l_type = lhs()->expr_type(), r_type = rhs()->expr_type();

    if ((l_type == FLOAT_TYPE) || (r_type == FLOAT_TYPE))
    {
        L = convert_value_to_float(L);
        R = convert_value_to_float(R);
        return Builder.CreateFCmpUGE(L, R);
    }

    if ((l_type == INT_TYPE) || (r_type == INT_TYPE))
    {
        L = convert_value_to_int(L);
        R = convert_value_to_int(R);
        return Builder.CreateICmpSGE(L, R);
    }

    // both bools
    return Builder.CreateICmpUGE(L, R);
}

Value *OpGT::codegen()
{
    auto L = lhs()->codegen(), R = rhs()->codegen();
    auto l_type = lhs()->expr_type(), r_type = rhs()->expr_type();

    if ((l_type == FLOAT_TYPE) || (r_type == FLOAT_TYPE))
    {
        L = convert_value_to_float(L);
        R = convert_value_to_float(R);
        return Builder.CreateFCmpUGT(L, R);
    }

    if ((l_type == INT_TYPE) || (r_type == INT_TYPE))
    {
        L = convert_value_to_int(L);
        R = convert_value_to_int(R);
        return Builder.CreateICmpSGT(L, R);
    }

    // both bools
    return Builder.CreateICmpUGT(L, R);
}

Value *OpLT::codegen()
{
    auto L = lhs()->codegen(), R = rhs()->codegen();
    auto l_type = lhs()->expr_type(), r_type = rhs()->expr_type();

    if ((l_type == FLOAT_TYPE) || (r_type == FLOAT_TYPE))
    {
        if (l_type == INT_TYPE)
        {
            L = convert_int_to_float(L);
        }
        else if (l_type == BOOL_TYPE)
        {
            L = convert_bool_to_float(L);
        }
        if (r_type == INT_TYPE)
        {
            R = convert_int_to_float(R);
        }
        else if (l_type == BOOL_TYPE)
        {
            R = convert_bool_to_float(R);
        }
        return Builder.CreateFCmpULT(L, R);
    }

    if ((l_type == INT_TYPE) || (r_type == INT_TYPE))
    {
        if (l_type == BOOL_TYPE)
        {
            L = Builder.CreateBitCast(L, Type::getInt32Ty(TheContext));
        }
        if (r_type == BOOL_TYPE)
        {
            R = Builder.CreateBitCast(R, Type::getInt32Ty(TheContext));
        }
        return Builder.CreateICmpSLT(L, R);
    }

    // both bools
    return Builder.CreateICmpULT(L, R);
}

char EqualityNode::expr_type()
{
    return BOOL_TYPE;
}

char OpMODULO::expr_type()
{
    return INT_TYPE;
}

char OpDIV::expr_type()
{
    auto l_type = lhs()->expr_type(), r_type = rhs()->expr_type();

    if ((l_type == FLOAT_TYPE) || (r_type == FLOAT_TYPE))
    {
        return FLOAT_TYPE;
    }
    return INT_TYPE;
}

char OpADD::expr_type()
{
    auto l_type = lhs()->expr_type(), r_type = rhs()->expr_type();

    if ((l_type == FLOAT_TYPE) || (r_type == FLOAT_TYPE))
    {
        return FLOAT_TYPE;
    }
    return INT_TYPE;
}

char OpMULT::expr_type()
{
    auto l_type = lhs()->expr_type(), r_type = rhs()->expr_type();

    if ((l_type == FLOAT_TYPE) || (r_type == FLOAT_TYPE))
    {
        return FLOAT_TYPE;
    }
    return INT_TYPE;
}

char OpSUB::expr_type()
{
    auto l_type = lhs()->expr_type(), r_type = rhs()->expr_type();

    if ((l_type == FLOAT_TYPE) || (r_type == FLOAT_TYPE))
    {
        return FLOAT_TYPE;
    }
    return INT_TYPE;
}

char InequalityNode::expr_type()
{
    return BOOL_TYPE;
}
