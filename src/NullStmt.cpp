#include "NullStmt.hpp"

extern LLVMContext TheContext;

Value *NullStmt::codegen()
{
	return ConstantInt::getTrue(TheContext);
}
