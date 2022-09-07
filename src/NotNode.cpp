#include "NotNode.hpp"

#include "the_externs.hpp"
#include "ValidTypes.hpp"
#include "conversions.hpp"

Value *NotNode::codegen()
{
	auto negatee_type = negatee()->expr_type();

	if (negatee_type == BOOL_TYPE)
	{
		auto ONE = ConstantInt::get(Type::getInt1Ty(TheContext), 1, false);
		return Builder.CreateXor(negatee()->codegen(), ONE, "nottemp");
	}
	else if (negatee_type == INT_TYPE)
	{
		auto current_parent = Builder.GetInsertBlock()->getParent();
		auto cmp            = int_cmp_zero(negatee()->codegen());

		auto then_block  = BasicBlock::Create(TheContext, "then", current_parent); // boolean false
		auto else_block  = BasicBlock::Create(TheContext, "else", current_parent); // boolean true
		auto merge_block = BasicBlock::Create(TheContext, "merge", current_parent);
		Builder.CreateCondBr(cmp, else_block, then_block);

		Builder.SetInsertPoint(then_block);
		auto then_v = ConstantInt::get(Type::getInt1Ty(TheContext), 1, false);
		Builder.CreateBr(merge_block);

		Builder.SetInsertPoint(else_block);
		auto else_v = ConstantInt::get(Type::getInt1Ty(TheContext), 1, false);
		Builder.CreateBr(merge_block);

		auto phi = Builder.CreatePHI(Type::getInt1Ty(TheContext), 2, "nottmp");
		phi->addIncoming(then_v, then_block);
		phi->addIncoming(else_v, else_block);
		return phi;
	}
	else if (negatee_type == FLOAT_TYPE)
	{
		auto Z          = ConstantFP::get(Type::getFloatTy(TheContext), 0.0);
		auto comparison = Builder.CreateFCmpUEQ(negatee()->codegen(), Z, "zcmp");
		auto B1         = BasicBlock::Create(TheContext, "zcmptrue");
		auto B2         = BasicBlock::Create(TheContext, "zcmpfalse");
		return Builder.CreateCondBr(comparison, B1, B2);
	}
	else
	{
		return nullptr;
	}
}

char NotNode::expr_type()
{
	return BOOL_TYPE;
}
