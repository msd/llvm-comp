#pragma once

#include "ASTnode.hpp"

class WhileStmt : public ASTnode
{
public:
	WhileStmt(unique_ptr<ASTnode> condition, unique_ptr<ASTnode> loop_body)
	{
		children.push_back(move(condition));
		children.push_back(move(loop_body));
	}

	ASTnode *condition()
	{
		return children[0].get();
	}

	ASTnode *loop_body()
	{
		return children[1].get();
	}

	virtual const string node_type() const
	{
		return "WHILE STMT";
	}

	virtual Value *codegen();
};
