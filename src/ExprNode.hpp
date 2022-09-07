#pragma once

#include "ASTnode.hpp"

class ExprNode : public ASTnode
{
public:
	virtual char expr_type() = 0;
	virtual Value *codegen() = 0;
};
