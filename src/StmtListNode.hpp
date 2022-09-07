#pragma once

#include "ASTnode.hpp"

class StmtListNode : public ASTnode
{
public:
	virtual const string node_type() const
	{
		return "STMTS";
	}

	virtual Value *codegen();
};
