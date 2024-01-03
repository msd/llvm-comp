#pragma once

#include "ASTnode.hpp"

class LocalDeclsNode : public ASTnode
{
  public:
    LocalDeclsNode(Parser *parser) : ASTnode(parser)
    {
    }
    virtual std::string node_type() const
    {
        return "LOCAL DECLS";
    }

    virtual Value *codegen();
};
