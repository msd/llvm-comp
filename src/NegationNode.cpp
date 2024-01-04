#include "NegationNode.hpp"

#include "the_externs.hpp"

#include <string>

#include "comparisons.hpp"

Value *NegationNode::codegen()
{
    auto negatee_type = negatee()->expr_type();

    if (negatee_type == INT_TYPE)
    {
        return Builder.CreateSub(IntValues::Zero(), negatee()->codegen());
    }
    if (negatee_type == FLOAT_TYPE)
    {
        return Builder.CreateFNeg(negatee()->codegen());
    }
    if (negatee_type == BOOL_TYPE)
    {
        auto C = Builder.CreateBitCast(negatee()->codegen(), IntValues::type());
        return Builder.CreateSub(IntValues::Zero(), C);
    }
    using std::to_string;
    throw compiler_error("cannot negate expr-type: " + to_string(negatee_type));
}
