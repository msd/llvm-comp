#pragma once

#include "ExprNode.hpp"
#include "VariableScope.hpp"
#include "AssignmentLHS.hpp"

class AssignmentExpr : public ExprNode
{
	VariableScope *scope;
public:
	AssignmentLHS *lhs()
	{
		return dynamic_cast<AssignmentLHS *>(children[0].get());
	}

	ExprNode *rhs()
	{
		return dynamic_cast<ExprNode *>(children[1].get());
	}

	virtual const string node_type() const
	{
		return "ASSIGNMENT STMT";
	}

	virtual char expr_type()
	{
		return rhs()->expr_type();
	}

	AssignmentExpr(VariableScope *scope, unique_ptr<ASTnode> lhs, unique_ptr<ASTnode> rhs);
	virtual Value *codegen();
};
