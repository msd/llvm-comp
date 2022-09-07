#include "AssignmentLHS.hpp"

extern IRBuilder<> Builder;

Value *AssignmentLHS::codegen()
{
	return Builder.getTrue(); // nothing to do
}
