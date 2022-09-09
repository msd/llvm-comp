#include "FunCallNode.hpp"

#include "FunDeclNode.hpp"
#include "my_errors.hpp"
#include "the_externs.hpp"

Value *FunCallNode::codegen()
{
    auto callee_function = TheModule->getFunction(fun_name);

    // todo when parsing check param names are unique

    return Builder.CreateCall(callee_function->getFunctionType(),
                              callee_function, {}, "funcalltmp");
    // TODO ASAP finish this
}

char FunCallNode::expr_type()
{
    if (DefinedFunctions.count(fun_name))
    {
        return DefinedFunctions[fun_name]->sig()->return_type;
    }
    throw semantic_error("Calling function " + fun_name +
                         " which hasn't been defined");
}
