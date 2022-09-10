#include "AssignmentLHS.hpp"

#include "the_externs.hpp"

Value *AssignmentLHS::codegen()
{
    return Builder.getTrue(); // nothing to do
}
