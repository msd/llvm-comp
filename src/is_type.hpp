#pragma once

#include "llvm/IR/Value.h"
#include "llvm/IR/Type.h"

using namespace llvm;

static bool is_int(Value *v)
{
	return v->getType()->isIntegerTy(32);
}


static bool is_bool(Value *v)
{
	return v->getType()->isIntegerTy(1);
}


static bool is_float(Value *v)
{
	return v->getType()->isFloatTy();
}
