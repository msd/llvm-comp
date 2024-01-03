#pragma once

#include <llvm/IR/Value.h>

extern llvm::Value *convert_float_to_int(llvm::Value *val);
extern llvm::Value *convert_bool_to_int(llvm::Value *val);
extern llvm::Value *convert_int_to_bool(llvm::Value *val);
extern llvm::Value *convert_float_to_bool(llvm::Value *val);
extern llvm::Value *convert_int_to_float(llvm::Value *val);
extern llvm::Value *convert_bool_to_float(llvm::Value *val);
extern llvm::Value *convert_value_to_int(llvm::Value *val);
extern llvm::Value *convert_value_to_float(llvm::Value *val);
extern llvm::Value *convert_value_to_bool(llvm::Value *val);

struct IntProxy
{
};
struct BoolProxy
{
};
struct FloatProxy
{
};

template <typename To> inline llvm::Value *convert_value_to(llvm::Value *value);
template <> inline llvm::Value *convert_value_to<IntProxy>(llvm::Value *value);
template <>
inline llvm::Value *convert_value_to<FloatProxy>(llvm::Value *value);
template <> inline llvm::Value *convert_value_to<BoolProxy>(llvm::Value *value);
