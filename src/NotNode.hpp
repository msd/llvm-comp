#pragma once

#include "RvalNode.hpp"

class NotNode : public RvalNode
{
public:
	NotNode(unique_ptr<ASTnode> sub)
	{
		addSub(move(sub));
	}

	RvalNode *negatee()
	{
		return dynamic_cast<RvalNode *>(children[0].get());
	}

	virtual const string node_type() const
	{
		return "NOT";
	}

	virtual char expr_type();

	virtual Value *codegen();
};
