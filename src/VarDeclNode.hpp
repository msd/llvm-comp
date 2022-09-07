#pragma once

#include "DeclNode.hpp"
#include "my_errors.hpp"

#include "ValidTypes.hpp"

class VariableScope;

class VarDeclNode : public DeclNode
{
public:
	VariableScope *scope;
	const string var_name;
	const char var_type;
	const bool is_global;
	VarDeclNode(VariableScope *scope, string var_name, const char var_type, bool is_global) : scope(scope), var_name(var_name), var_type(var_type), is_global(is_global) {}

	virtual const string node_type() const
	{
		return "VAR DECL";
	}

	virtual string to_string() const
	{
		return node_type() + " node NAME=" + var_name + " TYPE=" + type_to_str(var_type);
	}

	virtual Value *codegen();
};
