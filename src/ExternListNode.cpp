#include "ExternListNode.hpp"

#include "llvm/IR/IRBuilder.h"

extern IRBuilder<> Builder;

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
