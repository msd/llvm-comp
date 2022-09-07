#pragma once

#include "ValidTypes.hpp"
#include "LitNode.hpp"

class BoolNode : public LitNode<bool>
{
public:
	bool get_bool()
	{
		return *static_cast<bool *>(data);
	}

	virtual const string node_type() const
	{
		return "BOOL_LIT";
	}

	virtual Value *codegen();

	BoolNode(bool litVal) : LitNode<bool>(litVal) {}
	virtual char expr_type()
	{
		return BOOL_TYPE;
	}

	int as_int()
	{
		return get_bool()? 1: 0;
	}

	float as_float()
	{
		return static_cast<float>(get_bool());
	}
};
