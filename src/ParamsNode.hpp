#pragma once

#include "ParamNode.hpp"

class ParamsNode : public ASTnode
{
public:
	ParamNode *get(int i)
	{
		return dynamic_cast<ParamNode *>(children[i].get());
	}

	int param_count()
	{
		return children.size();
	}

	virtual const string node_type() const
	{
		return "PARAMS";
	}

	virtual Value *codegen();
};
