#pragma once

#include "conversions.hpp"

extern Value *convert_float_to_int(Value *val);
extern Value *convert_bool_to_int(Value *val);
extern Value *convert_int_to_bool(Value *val);
extern Value *convert_float_to_bool(Value *val);
extern Value *convert_int_to_float(Value *val);
extern Value *convert_bool_to_float(Value *val);
extern Value *convert_value_to_int(Value *val);
extern Value *convert_value_to_float(Value *val);
extern Value *convert_value_to_bool(Value *val);
