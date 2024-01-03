#pragma once

#include "llvm/IR/Type.h"
#include "llvm/IR/Value.h"

#include "ExprNode.hpp"

using namespace llvm;

bool is_int(Value *v);

bool is_bool(Value *v);

bool is_float(Value *v);

template <typename T>
Value *create_true_cmp(Value *expr, Twine const &name = "");
template <> Value *create_true_cmp<int>(Value *expr, Twine const &name);
template <> Value *create_true_cmp<float>(Value *expr, Twine const &name);
template <> Value *create_true_cmp<bool>(Value *expr, Twine const &name);

Value *create_expr_true_check(ExprNode *expr);
