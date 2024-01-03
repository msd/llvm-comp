#pragma once

#include "llvm/IR/Value.h"

#include "ExprNode.hpp"

using namespace llvm;

int const constexpr INT_BITS_COUNT = 32;
int const constexpr BOOL_BITS_COUNT = 1;

[[nodiscard]] bool is_int(Value *v);
[[nodiscard]] bool is_bool(Value *v);
[[nodiscard]] bool is_float(Value *v);
