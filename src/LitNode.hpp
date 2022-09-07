#pragma once

#include "RvalNode.hpp"

template<typename T>
class LitNode : public RvalNode
{
public:
	T *data;
	LitNode(T litVal)
	{
		data  = new T;
		*data = litVal;
	}

	virtual string data_str() const
	{
		return std::to_string(*data);
	}

	virtual string to_string() const
	{
		return node_type() + "=" + data_str();
	}

	~LitNode()
	{
		delete data;
	}

	virtual char expr_type() = 0;
};
