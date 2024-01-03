#pragma once

#include "LitNode.hpp"
#include "ValidTypes.hpp"

class BoolNode : public LitNode<bool>
{
  public:
    bool get_bool()
    {
        return data;
    }

    virtual const std::string node_type() const
    {
        return "BOOL_LIT";
    }

    virtual Value *codegen();

    BoolNode(Parser *parser, bool litVal) : LitNode(parser, litVal)
    {
    }
    virtual char expr_type()
    {
        return BOOL_TYPE;
    }

    int as_int()
    {
        return get_bool() ? 1 : 0;
    }

    virtual ~BoolNode()
    {
    }

    float as_float()
    {
        return static_cast<float>(get_bool());
    }
};
