#include "IfStmt.hpp"

#include "the_externs.hpp"

Value *IfStmt::codegen()
{
	auto  outer_function = Builder.GetInsertBlock()->getParent();
	auto  cond_type      = cond->expr_type();
	Value *comparison;

	if (cond_type == INT_TYPE)
	{
		auto Z = ConstantInt::get(Type::getInt32Ty(TheContext), 0, true);
		comparison = Builder.CreateICmpNE(cond->codegen(), Z);
	}
	else if (cond_type == FLOAT_TYPE)
	{
		auto Z = ConstantFP::get(Type::getFloatTy(TheContext), 0.0);
		comparison = Builder.CreateFCmpUNE(cond->codegen(), Z);
	}
	else if (cond_type == BOOL_TYPE)
	{
		auto Z = ConstantInt::get(Type::getInt1Ty(TheContext), 0, false);
		comparison = Builder.CreateICmpNE(cond->codegen(), Z);
	}
	else
	{
		return nullptr;
	}
	auto then_block = BasicBlock::Create(TheContext, "then", outer_function);
	auto after_if   = BasicBlock::Create(TheContext, "ifcont", outer_function);

	Builder.CreateCondBr(comparison, then_block, after_if);

	Builder.SetInsertPoint(then_block);
	then_body->codegen();
	Builder.CreateBr(after_if);

	outer_function->getBasicBlockList().push_back(after_if);
	Builder.SetInsertPoint(after_if);
	return Builder.getTrue();
}
