#pragma once

#include "ASTnode.hpp"

class ReturnNothingNode : public ASTnode
{
  public:
    ReturnNothingNode(Parser *parser) : ASTnode(parser)
    {
    }
    virtual const std::string node_type() const
    {
        return "RETURN NOTHING STMT";
    }

    virtual Value *codegen();
};
