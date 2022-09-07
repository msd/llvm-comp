#include "NegationNode.hpp"

#include "the_externs.hpp"

Value *NegationNode::codegen()
{
	auto negatee_type = negatee()->expr_type();

	if (negatee_type == INT_TYPE)
	{
		auto Z = ConstantInt::get(Type::getInt32Ty(TheContext), 0, true);
		return Builder.CreateSub(Z, negatee()->codegen());
	}
	else if (negatee_type == FLOAT_TYPE)
	{
		return Builder.CreateFNeg(negatee()->codegen());
	}
	else if (negatee_type == BOOL_TYPE)
	{
		auto C = Builder.CreateBitCast(negatee()->codegen(), Type::getInt32Ty(TheContext));
		auto Z = ConstantInt::get(Type::getInt32Ty(TheContext), 0, true);
		return Builder.CreateSub(Z, C);
	}
	else
	{
		return nullptr;
	}
}
