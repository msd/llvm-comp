#include "ProgramNode.hpp"

Value *ProgramNode::codegen()
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
