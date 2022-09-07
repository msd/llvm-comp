#pragma once

#include "ASTnode.hpp"

class NullStmt : public ASTnode
{
public:
	virtual const string node_type() const
	{
		return "NULL STMT";
	}

	virtual Value *codegen();
};
