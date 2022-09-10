#pragma once

#include "llvm/IR/IRBuilder.h"

using namespace llvm;

Value *int_cmp_zero(Value *val);

Value *float_cmp_zero(Value *val);
