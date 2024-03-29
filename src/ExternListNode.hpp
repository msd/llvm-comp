#pragma once

#include "ASTnode.hpp"

class ExternListNode : public ASTnode
{
  public:
    ExternListNode(Parser *parser) : ASTnode(parser)
    {
    }
    virtual std::string node_type() const
    {
        return "EXTERN LIST";
    }

    virtual Value *codegen();
};
