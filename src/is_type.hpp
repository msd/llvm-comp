#pragma once

#include "llvm/IR/Type.h"
#include "llvm/IR/Value.h"

#include "ExprNode.hpp"

using namespace llvm;

bool is_int(Value *v);

bool is_bool(Value *v);

bool is_float(Value *v);

Value *create_expr_true_check(ExprNode *expr);
