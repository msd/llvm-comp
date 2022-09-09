#pragma once

#include "ExprNode.hpp"

class RvalNode : public ExprNode
{
  public:
    virtual ~RvalNode()
    {
    }
    virtual char expr_type() = 0;
    virtual const string node_type() const = 0;
    virtual Value *codegen() = 0;
};
