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

template <typename To> inline llvm::Value *convert_value_to(llvm::Value *value);
template <> inline llvm::Value *convert_value_to<int>(llvm::Value *value);
template <> inline llvm::Value *convert_value_to<float>(llvm::Value *value);
template <> inline llvm::Value *convert_value_to<bool>(llvm::Value *value);
