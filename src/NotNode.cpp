#include "NotNode.hpp"

#include "ValidTypes.hpp"
#include "coercion.hpp"
#include "comparisons.hpp"
#include "the_externs.hpp"

Value *NotNode::codegen()
{
    auto V = convert_value_to_bool(negatee()->codegen());
    auto T = BoolValues::True();
    return Builder.CreateXor(V, T, "notResult");
}

char NotNode::expr_type()
{
    return BOOL_TYPE;
}
