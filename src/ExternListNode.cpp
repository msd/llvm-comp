#include "ExternListNode.hpp"

#include "llvm/IR/IRBuilder.h"

#include "the_externs.hpp"

Value *ExternListNode::codegen()
{
    if (children.size())
    {
        Value *v;

        for (auto &c : children)
        {
            v = c->codegen();
            if (!v)
            {
                return nullptr;
            }
        }
        return v;
    }
    return Builder.getTrue();
}
