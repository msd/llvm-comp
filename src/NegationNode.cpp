#include "NegationNode.hpp"

#include <string>

#include "comparisons.hpp"
#include "the_externs.hpp"

Value *NegationNode::codegen()
{
    auto negatee_type = negatee()->expr_type();

    auto V = negatee()->codegen();

    if (negatee_type == INT_TYPE)
    {
        return Builder.CreateSub(IntValues::Zero(), V);
    }
    if (negatee_type == FLOAT_TYPE)
    {
        return Builder.CreateFNeg(V);
    }
    if (negatee_type == BOOL_TYPE)
    {
        return Builder.CreateSub(IntValues::Zero(), IntValues::fromBoolV(V));
    }
    using std::to_string;
    throw compiler_error("cannot negate expr-type: " + to_string(negatee_type));
}
