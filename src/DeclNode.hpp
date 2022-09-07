#pragma once

#include "ASTnode.hpp"

class DeclNode : public ASTnode
{
	virtual const string node_type() const = 0;
};
