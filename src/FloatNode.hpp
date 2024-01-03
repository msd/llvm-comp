#pragma once

#include "LitNode.hpp"
#include "ValidTypes.hpp"

class FloatNode : public LitNode<float>
{
  public:
    FloatNode(Parser *parser, float litVal) : LitNode<float>(parser, litVal)
    {
    }

    virtual const std::string node_type() const
    {
        return "FLOAT_LIT";
    }

    float get_float()
    {
        return data;
    }

    virtual ~FloatNode()
    {
    }

    virtual Value *codegen();

    virtual char expr_type()
    {
        return FLOAT_TYPE;
    }
};
