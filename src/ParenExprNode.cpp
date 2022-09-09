#include "ParenExprNode.hpp"

Value *ParenExprNode::codegen()
{
    return expr()->codegen();
}
