#include "DeclListNode.hpp"

Value *DeclListNode::codegen()
{
    Value *v;

    for (int i = 0; i < children.size(); ++i)
    {
        v = children[i]->codegen();
        if (!v)
        {
            return nullptr;
        }
    }
    return v;
}
