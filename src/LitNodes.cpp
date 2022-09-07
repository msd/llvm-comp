#include "IntNode.hpp"
#include "FloatNode.hpp"
#include "BoolNode.hpp"

#include "the_externs.hpp"

Value *IntNode::codegen()
{
	return ConstantInt::get(Type::getInt32Ty(TheContext), get_int(), true);
}


Value *BoolNode::codegen()
{
	return ConstantInt::get(Type::getInt1Ty(TheContext), get_bool()?1:0, false);
}


Value *FloatNode::codegen()
{
	return ConstantFP::get(Type::getFloatTy(TheContext), get_float());
}
