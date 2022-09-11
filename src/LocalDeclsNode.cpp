#include "LocalDeclsNode.hpp"

#include "the_externs.hpp"

Value *LocalDeclsNode::codegen()
{
    if (children.size())
    {
        Value *v;

        for (int i = 0; i < children.size(); ++i)
        {
            v = children[i]->codegen();
            if (!v)
            {
                throw compiler_error("failed to allocate create local variable",
                                     &token);
            }
        }
        return v;
    }
    return Builder.getTrue();
}
