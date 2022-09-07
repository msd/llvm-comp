#pragma once

#include "ASTnode.hpp"

class LocalDeclsNode : public ASTnode
{
public:
	virtual const string node_type() const
	{
		return "LOCAL DECLS";
	}

	virtual Value *codegen();
};
