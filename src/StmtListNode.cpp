#include "StmtListNode.hpp"

#include "the_externs.hpp"

Value *StmtListNode::codegen()
{
    if (children.size())
    {
        Value *v;

        for (int i = 0; i < children.size(); ++i)
        {
            v = children[i]->codegen();
            if (!v)
            {
                throw compiler_error("failed to codegen in statement list");
            }
        }
        return v;
    }
    return Builder.getTrue();
}
