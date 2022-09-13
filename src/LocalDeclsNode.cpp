#include "LocalDeclsNode.hpp"

#include "the_externs.hpp"

Value *LocalDeclsNode::codegen()
{
    if (!children.size())
    {
        return Builder.getTrue();
    }

    Value *v;
    for (auto &child : children)
    {
        v = child->codegen();
        if (!v)
        {
            throw compiler_error("failed to allocate create local variable",
                                 &token);
        }
    }
    return v;
}
