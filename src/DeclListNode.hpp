#pragma once

#include "ASTnode.hpp"

class DeclListNode : public ASTnode
{
  public:
    explicit DeclListNode(Parser *parser) : ASTnode(parser)
    {
    }
    virtual std::string node_type() const
    {
        return "DECL LIST";
    }

    virtual Value *codegen();
};
