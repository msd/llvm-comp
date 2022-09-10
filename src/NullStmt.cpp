#include "NullStmt.hpp"

#include "the_externs.hpp"

Value *NullStmt::codegen()
{
    return ConstantInt::getTrue(TheContext);
}
