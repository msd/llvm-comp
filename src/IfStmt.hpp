#pragma once

#include "ASTnode.hpp"
#include "ExprNode.hpp"
#include "BlockNode.hpp"

class IfStmt : public ASTnode
{
public:
	ExprNode *cond;
	BlockNode *then_body;
	IfStmt(unique_ptr<ASTnode> if_cond, unique_ptr<ASTnode> if_body)
	{
		addSub(move(if_cond));
		addSub(move(if_body));
		cond      = dynamic_cast<ExprNode *>(children[0].get());
		then_body = dynamic_cast<BlockNode *>(children[1].get());
	}

	virtual const string node_type() const
	{
		return "IF STMT";
	}

	virtual Value *codegen();
};
