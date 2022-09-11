#pragma once

#include "RvalNode.hpp"

class FunCallNode : public RvalNode
{
  public:
    const string fun_name;
    FunCallNode(Parser *parser, string fun_name)
        : RvalNode(parser), fun_name(fun_name)
    {
    }
    virtual const string node_type() const
    {
        return "FUNCTION CALL";
    }

    virtual string to_string() const
    {
        return node_type() + " node CALLEE=" + fun_name;
    }

    virtual ~FunCallNode()
    {
    }

    virtual char expr_type();
    virtual Value *codegen();
};
