#include "StmtListNode.hpp"

#include "the_externs.hpp"

Value *StmtListNode::codegen()
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
            throw compiler_error("failed to codegen in statement list");
        }
    }
    return v;
}
