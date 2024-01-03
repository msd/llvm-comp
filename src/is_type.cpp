#include "is_type.hpp"

#include "the_externs.hpp"

[[nodiscard]] bool is_int(Value *v)
{
    return v->getType()->isIntegerTy(INT_BITS_COUNT);
}

[[nodiscard]] bool is_bool(Value *v)
{
    return v->getType()->isIntegerTy(BOOL_BITS_COUNT);
}

[[nodiscard]] bool is_float(Value *v)
{
    return v->getType()->isFloatTy();
}
