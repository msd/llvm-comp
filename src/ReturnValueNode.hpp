#pragma once

#include "ExprNode.hpp"

class ReturnValueNode : public ASTnode
{
public:
	ExprNode *expr;
	ReturnValueNode(unique_ptr<ASTnode> expr)
	{
		children.push_back(move(expr));

		this->expr = dynamic_cast<ExprNode *>(children[0].get());
	}

	virtual const string node_type() const
	{
		return "RETURN VALUE STMT";
	}

	virtual Value *codegen();
};
