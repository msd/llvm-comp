#pragma once

#include "LitNode.hpp"
#include "ValidTypes.hpp"

class Parser;

/// IntASTnode - Class for integer literals like 1, 2, 10,
class IntNode : public LitNode<int>
{
  public:
    IntNode(Parser *parser, int litVal) : LitNode<int>(parser, litVal)
    {
    }
    virtual const string node_type() const
    {
        return "INT_LIT";
    }

    int get_int()
    {
        return data;
    }

    virtual Value *codegen();

    virtual ~IntNode()
    {
    }

    virtual char expr_type()
    {
        return INT_TYPE;
    }

    float as_float()
    {
        return static_cast<float>(get_int());
    }
};
