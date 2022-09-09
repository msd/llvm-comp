#include "ReturnNothingNode.hpp"
#include "the_externs.hpp"

Value *ReturnNothingNode::codegen()
{
    return Builder.CreateRetVoid();
}
