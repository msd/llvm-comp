#include "FunDeclNode.hpp"
#include "llvm/IR/Verifier.h"

#include "the_externs.hpp"

Function *FunDeclNode::codegen()
{
    Function *the_function = TheModule->getFunction(sig()->name);

    if (!the_function)
    {
        the_function = sig()->codegen();
    }

    if (!the_function)
    {
        return nullptr;
    }

    if (!the_function->empty())
    {
        throw semantic_error("Function " + sig()->name +
                             " cannot be  redefined");
    }

    auto BB = BasicBlock::Create(TheContext, "entry", the_function);

    Builder.SetInsertPoint(BB);

    // clear named values map
    // for (arg in args) set name in values map

    body()->codegen();

    verifyFunction(*the_function);

    return the_function;
}
