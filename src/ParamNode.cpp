#include "ParamNode.hpp"

#include "the_externs.hpp"

Value *ParamNode::codegen()
{
	return nullptr;
}

Type *ParamNode::llvm_type()
{
	switch (type)
	{
	case FLOAT_TYPE:
		return Type::getFloatTy(TheContext);

	case BOOL_TYPE:
		return Type::getInt1Ty(TheContext);

	case INT_TYPE:
		return Type::getInt32Ty(TheContext);

	default:
		return nullptr;
	}
}
