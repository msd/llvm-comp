#pragma once

#include "LitNode.hpp"
#include "ValidTypes.hpp"

class FloatNode : public LitNode<float>
{
  public:
    FloatNode(float litVal) : LitNode<float>(litVal)
    {
    }
    virtual string to_string() const
    {
        return node_type() + "=" + std::to_string(*static_cast<float *>(data));
    }

    virtual const string node_type() const
    {
        return "FLOAT_LIT";
    }

    float get_float()
    {
        return *static_cast<float *>(data);
    }

    virtual Value *codegen();

    virtual char expr_type()
    {
        return FLOAT_TYPE;
    }
};
