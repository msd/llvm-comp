#include "ReturnValueNode.hpp"

#include "coercion.hpp"
#include "my_errors.hpp"
#include "the_externs.hpp"

Value *ReturnValueNode::codegen()
{
    // todo jump up to function definition to find return type
    auto current_function = Builder.GetInsertBlock()->getParent();
    auto return_type = current_function->getFunctionType()->getReturnType();

    if (return_type->isVoidTy())
    {
        throw semantic_error("Function declared as void cannot return a value",
                             &token);
    }

    auto return_val = expr->codegen();
    Value *R;

    if (return_type->isIntegerTy(32))
    {
        R = convert_value_to_int(return_val);
    }
    else if (return_type->isIntegerTy(1))
    {
        R = convert_value_to_bool(return_val);
    }
    else if (return_type->isFloatTy())
    {
        R = convert_value_to_float(return_val);
    }
    else
    {
        throw compiler_error("COMPILER ERROR: 651", &token);
    }

    return Builder.CreateRet(R);
}
