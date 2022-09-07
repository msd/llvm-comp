#pragma once

#include "ExternListNode.hpp"
#include "DeclListNode.hpp"

class ProgramNode : public ASTnode
{
public:
	ExternListNode *extern_list;
	DeclListNode *decl_list;
	ProgramNode(unique_ptr<ExternListNode> eln, unique_ptr<DeclListNode> dln)
	{
		children.push_back(move(eln));
		children.push_back(move(dln));
		extern_list = dynamic_cast<ExternListNode *>(children[0].get());
		decl_list   = dynamic_cast<DeclListNode *>(children[1].get());
	}

	virtual const string node_type() const
	{
		return "PROGRAM";
	}

	virtual Value *codegen();
};
