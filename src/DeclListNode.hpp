#pragma once

#include "ASTnode.hpp"

class DeclListNode : public ASTnode
{
  public:
    DeclListNode(Parser *parser) : ASTnode(parser)
    {
    }
    virtual const string node_type() const
    {
        return "DECL LIST";
    }

    virtual Value *codegen();
};
