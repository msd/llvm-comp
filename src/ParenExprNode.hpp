#pragma once

#include "RvalNode.hpp"

class ParenExprNode : public RvalNode
{
public:
	ParenExprNode(unique_ptr<ASTnode> expr)
	{
		children.push_back(move(expr));
	}

	RvalNode *expr()
	{
		return dynamic_cast<RvalNode *>(children[0].get());
	}

	virtual const string node_type() const
	{
		return "PAREN EXPR";
	}

	virtual char expr_type()
	{
		return expr()->expr_type();
	}

	virtual Value *codegen();
};
