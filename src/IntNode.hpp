#pragma once

#include "LitNode.hpp"
#include "ValidTypes.hpp"

/// IntASTnode - Class for integer literals like 1, 2, 10,
class IntNode : public LitNode<int>
{
  public:
    IntNode(int litVal) : LitNode<int>(litVal)
    {
    }
    virtual const string node_type() const
    {
        return "INT_LIT";
    }

    int get_int()
    {
        return *static_cast<int *>(data);
    }

    virtual Value *codegen();

    virtual char expr_type()
    {
        return INT_TYPE;
    }

    float as_float()
    {
        return static_cast<float>(get_int());
    }
};
