#include "DeclListNode.hpp"

#include "my_errors.hpp"

Value *DeclListNode::codegen()
{
    Value *v;

    for (int i = 0; i < children.size(); ++i)
    {
        v = children[i]->codegen();
        if (!v)
        {
            throw compiler_error("decl statement could not be created", &token);
        }
    }
    return v;
}
