#pragma once

#include "llvm/IR/Value.h"
#include <cstdint>
#include <llvm/IR/IRBuilder.h>

#include "ExprNode.hpp"
#include "the_externs.hpp"

using namespace llvm;

Value *int_cmp_zero(Value *val);

Value *float_cmp_zero(Value *val);

Type *type_homebrew_to_llvm(char type);

template <typename T>
[[nodiscard]] Value *create_true_cmp(Value *expr, Twine const &name = "");
template <>
[[nodiscard]] Value *create_true_cmp<int>(Value *expr, Twine const &name);
template <>
[[nodiscard]] Value *create_true_cmp<float>(Value *expr, Twine const &name);
template <>
[[nodiscard]] Value *create_true_cmp<bool>(Value *expr, Twine const &name);

struct BoolValues
{
    [[nodiscard]] inline static auto type()
    {
        return Type::getInt1Ty(TheContext);
    }

    [[nodiscard]] inline static auto get(uint64_t x)
    {
        return ConstantInt::get(Type::getInt1Ty(TheContext), x, false);
    }

    [[nodiscard]] inline static auto True()
    {
        return get(1);
    }

    [[nodiscard]] inline static auto False()
    {
        return get(0);
    }
};

struct IntValues
{
    [[nodiscard]] inline static auto type()
    {
        return Type::getInt32Ty(TheContext);
    }

    [[nodiscard]] inline static auto get(uint64_t x, bool signed_)
    {
        return ConstantInt::get(type(), x, signed_);
    }

    [[nodiscard]] inline static auto getSigned(uint64_t x)
    {
        return get(x, true);
    }

    [[nodiscard]] inline static auto getUnsigned(uint64_t x)
    {
        return get(x, false);
    }

    [[nodiscard]] inline static auto Zero()
    {
        return getUnsigned(0);
    }

    [[nodiscard]] inline static auto One()
    {
        return getUnsigned(1);
    }
};

struct FloatValues
{
    [[nodiscard]] inline static auto type()
    {
        return Type::getFloatTy(TheContext);
    }

    [[nodiscard]] inline static auto get(double x)
    {
        return ConstantFP::get(type(), x);
    }

    [[nodiscard]] inline static auto Zero()
    {
        return get(0);
    }

    [[nodiscard]] inline static auto One()
    {
        return get(1);
    }
};

[[nodiscard]] Value *create_expr_true_check(ExprNode *expr);
