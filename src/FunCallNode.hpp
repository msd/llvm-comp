#pragma once

#include "RvalNode.hpp"

class FunCallNode : public RvalNode
{
  public:
    const std::string fun_name;
    FunCallNode(Parser *parser, std::string fun_name)
        : RvalNode(parser), fun_name(fun_name)
    {
    }
    virtual std::string node_type() const
    {
        return "FUNCTION CALL";
    }

    virtual std::string to_string() const
    {
        return node_type() + " node CALLEE=" + fun_name;
    }

    virtual ~FunCallNode()
    {
    }

    virtual char expr_type();
    virtual Value *codegen();
};
