#include "coercion.hpp"
#include "my_errors.hpp"
#include "the_externs.hpp"

Value *convert_float_to_int(Value *val)
{
	return Builder.CreateFPToSI(val, Type::getInt32Ty(TheContext), "float2inttmp");
}

Value *convert_bool_to_int(Value *val)
{
	return Builder.CreateZExt(val, Type::getInt32Ty(TheContext), "bool2inttmp");
}

Value *convert_int_to_bool(Value *val)
{
	auto current_parent = Builder.GetInsertBlock()->getParent();
	auto cmp            = Builder.CreateICmpEQ(val, Builder.getFalse(), "zcmptmp");
	auto then_block     = BasicBlock::Create(TheContext, "then", current_parent);
	auto else_block     = BasicBlock::Create(TheContext, "else", current_parent);
	auto merge_block    = BasicBlock::Create(TheContext, "ifcont", current_parent);

	Builder.CreateCondBr(cmp, then_block, else_block);

	Builder.SetInsertPoint(then_block);
	auto then_v = ConstantInt::get(Type::getInt1Ty(TheContext), 0, true);

	Builder.CreateBr(merge_block);

	Builder.SetInsertPoint(else_block);
	auto else_v = ConstantInt::get(Type::getInt1Ty(TheContext), 1, true);

	Builder.CreateBr(merge_block);

	Builder.SetInsertPoint(merge_block);
	auto phi = Builder.CreatePHI(Type::getInt1Ty(TheContext), 2, "int2booltmp");

	phi->addIncoming(then_v, then_block);
	phi->addIncoming(else_v, else_block);

	return phi;
}

Value *convert_float_to_bool(Value *val)
{
	auto current_parent = Builder.GetInsertBlock()->getParent();
	auto cmp            = float_cmp_zero(val);
	auto then_block     = BasicBlock::Create(TheContext, "then", current_parent);
	auto else_block     = BasicBlock::Create(TheContext, "else", current_parent);
	auto merge_block    = BasicBlock::Create(TheContext, "ifcont", current_parent);

	Builder.CreateCondBr(cmp, then_block, else_block);


	Builder.SetInsertPoint(then_block);
	auto then_v = ConstantInt::get(Type::getInt1Ty(TheContext), 0.0f);

	Builder.CreateBr(merge_block);

	Builder.SetInsertPoint(else_block);
	auto else_v = ConstantInt::get(Type::getInt1Ty(TheContext), 1.0f);

	Builder.CreateBr(merge_block);

	Builder.SetInsertPoint(merge_block);
	auto phi = Builder.CreatePHI(Type::getInt1Ty(TheContext), 2, "float2booltmp");

	phi->addIncoming(then_v, then_block);
	phi->addIncoming(else_v, else_block);

	return phi;
}

Value *convert_int_to_float(Value *val)
{
	return Builder.CreateSIToFP(val, Type::getFloatTy(TheContext), "int2floattmp");
}

Value *convert_bool_to_float(Value *val)
{
	auto current_parent = Builder.GetInsertBlock()->getParent();
	auto cmp            = val;
	auto then_block     = BasicBlock::Create(TheContext, "then", current_parent);
	auto else_block     = BasicBlock::Create(TheContext, "else", current_parent);
	auto merge_block    = BasicBlock::Create(TheContext, "ifcont", current_parent);

	Builder.CreateCondBr(cmp, then_block, else_block);

	Builder.SetInsertPoint(then_block);
	auto then_v = ConstantFP::get(Type::getFloatTy(TheContext), 1.0f);

	Builder.CreateBr(merge_block);

	Builder.SetInsertPoint(else_block);
	auto else_v = ConstantFP::get(Type::getFloatTy(TheContext), 0.0f);

	Builder.CreateBr(merge_block);

	Builder.SetInsertPoint(merge_block);
	auto phi = Builder.CreatePHI(Type::getFloatTy(TheContext), 2, "bool2floattmp");

	phi->addIncoming(then_v, then_block);
	phi->addIncoming(else_v, else_block);

	return phi;
}

Value *convert_value_to_int(Value *val)
{
	auto val_type = val->getType();

	if (val_type->isIntegerTy(1))
	{
		return convert_bool_to_int(val);
	}
	if (val_type->isIntegerTy(32))
	{
		return val;
	}
	if (val_type->isFloatTy())
	{
		return convert_float_to_int(val);
	}
	throw compiler_error("unknown type to convert to int");
}

Value *convert_value_to_float(Value *val)
{
	auto val_type = val->getType();

	if (val_type->isIntegerTy(1))
	{
		return convert_bool_to_float(val);
	}
	if (val_type->isIntegerTy(32))
	{
		return convert_int_to_float(val);
	}
	if (val_type->isFloatTy())
	{
		return val;
	}
	throw compiler_error("unknown type to convert to float");
}


Value *convert_value_to_bool(Value *val)
{
	auto val_type = val->getType();

	if (val_type->isIntegerTy(1))
	{
		return val;
	}
	if (val_type->isIntegerTy(32))
	{
		return convert_int_to_bool(val);
	}
	if (val_type->isFloatTy())
	{
		return convert_float_to_bool(val);
	}
	throw compiler_error("unknown type to convert to bool");
}
