#pragma once

#include "ASTnode.hpp"

class ReturnNothingNode : public ASTnode
{
public:
	virtual const string node_type() const
	{
		return "RETURN NOTHING STMT";
	}

	virtual Value *codegen();
};
