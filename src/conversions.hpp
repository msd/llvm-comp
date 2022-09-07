#pragma once

#include "llvm/IR/IRBuilder.h"

using namespace llvm;

extern LLVMContext TheContext;
extern IRBuilder<> Builder;

static Value *int_cmp_zero(Value *val)
{
	auto Z = ConstantInt::get(Type::getInt32Ty(TheContext), 0, true);

	return Builder.CreateICmpEQ(val, Z, "intzcmptmp");
}


static Value *float_cmp_zero(Value *val)
{
	auto Z = ConstantFP::get(Type::getFloatTy(TheContext), 0.0f);

	return Builder.CreateFCmpUEQ(val, Z, "floatzcmptmp");
}
