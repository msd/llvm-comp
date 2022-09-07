#include "DisCon.hpp"
#include "my_errors.hpp"
#include "coercion.hpp"

#include "the_externs.hpp"
#include "ValidTypes.hpp"

// Short-circuit evaluation of conjunction (boolean AND)
Value *ConjunctionNode::codegen()
{
	if (children.size() == 1)
	{
		return children[0].get()->codegen();
	}
	auto  current_parent = Builder.GetInsertBlock()->getParent();
	Value *Result        = CreateEntryBlockAlloca(Type::getInt1Ty(TheContext), "conj_result");
	auto  end_block      = BasicBlock::Create(TheContext, "conj_ret");
	auto  succ_block     = BasicBlock::Create(TheContext, "conj_succ");
	auto  next_block     = BasicBlock::Create(TheContext, "conj_cl2");

	// Short-circuit evaluaton. The result is presumed false and
	// only changed to true after all nodes have been visited. If
	// any of the nodes (i.e. clauses of the conjunction) are false
	// then the result is immediately returned on the discovery of
	// such node. At the end of none such clauses are found then
	// Result is changed to true and then returned.
	Builder.CreateStore(Builder.getFalse(), Result);

	// First iteration on first clause of disjunction (i.e. first clause of disjunction)
	auto current_block = BasicBlock::Create(TheContext, "conj_cl1");

	Builder.CreateCondBr(convert_value_to_bool(children[0]->codegen()), end_block, next_block);

	// Iterate over all inner elements
	for (int i = 0; i < children.size() - 1; ++i)
	{
		current_parent->getBasicBlockList().push_back(current_block);                                   // add previous block
		current_block = next_block;
		next_block    = BasicBlock::Create(TheContext, string("conj_cl") + std::to_string(i + 1)); // last claus in disjunction
		Builder.CreateCondBr(convert_value_to_bool(children[i]->codegen()), next_block, end_block);
	}

	// Last iteration, only happens for 2 or more elements
	if (children.size() > 1)
	{
		Builder.SetInsertPoint(next_block);
		Builder.CreateCondBr(convert_value_to_bool(children[children.size() - 1]->codegen()), end_block, succ_block);
		current_parent->getBasicBlockList().push_back(next_block);
	}
	// TODO check actually generates the right clause labels numbers 2..n (n=number of clauses in disjunction)

	// fail block sets return value to 0
	Builder.SetInsertPoint(succ_block);
	Builder.CreateStore(Builder.getFalse(), Result);
	Builder.CreateBr(end_block);
	current_parent->getBasicBlockList().push_back(succ_block);

	// finally read the value and return it
	Builder.SetInsertPoint(end_block);
	auto ret_tmp = Builder.CreateLoad(Type::getInt1Ty(TheContext), Result, "conj_val");

	current_parent->getBasicBlockList().push_back(end_block);
	return ret_tmp;
}


// Short-circuit evaluation of disjunction (boolean OR)
Value *DisjunctionNode::codegen()
{
	if (children.size() == 1)
	{
		return children[0].get()->codegen();
	}
	auto  current_parent = Builder.GetInsertBlock()->getParent();
	Value *Result        = CreateEntryBlockAlloca(Type::getInt1Ty(TheContext), "disj_result");
	auto  end_block      = BasicBlock::Create(TheContext, "disj_ret");
	auto  fail_block     = BasicBlock::Create(TheContext, "disj_fail");
	auto  next_block     = BasicBlock::Create(TheContext, "disj_cl2");

	Builder.CreateStore(Builder.getTrue(), Result);

	// First iteration on first clause of disjunction (i.e. first clause of disjunction)
	auto current_block = BasicBlock::Create(TheContext, "disj_cl1");

	Builder.CreateCondBr(convert_value_to_bool(children[0]->codegen()), end_block, next_block);

	// Iterate over all inner elements
	for (int i = 0; i < children.size() - 1; ++i)
	{
		current_parent->getBasicBlockList().push_back(current_block);                                   // add previous block
		current_block = next_block;
		next_block    = BasicBlock::Create(TheContext, string("disj_cl") + std::to_string(i + 1)); // last claus in disjunction
		Builder.CreateCondBr(convert_value_to_bool(children[i]->codegen()), end_block, next_block);
	}

	// Last iteration, only happens for 2 or more elements
	if (children.size() > 1)
	{
		Builder.SetInsertPoint(next_block);
		Builder.CreateCondBr(convert_value_to_bool(children[children.size() - 1]->codegen()), end_block, fail_block);
		current_parent->getBasicBlockList().push_back(next_block);
	}
	// TODO check actually generates the right clause labels numbers 2..n (n=number of clauses in disjunction)

	// fail block sets return value to 0
	Builder.SetInsertPoint(fail_block);
	Builder.CreateStore(Builder.getFalse(), Result);
	Builder.CreateBr(end_block);
	current_parent->getBasicBlockList().push_back(fail_block);

	// finally read the value and return it
	Builder.SetInsertPoint(end_block);
	auto ret_tmp = Builder.CreateLoad(Type::getInt1Ty(TheContext), Result, "disjval");

	current_parent->getBasicBlockList().push_back(end_block);
	return ret_tmp;
}

char DisCon::expr_type()
{
	return BOOL_TYPE;
}